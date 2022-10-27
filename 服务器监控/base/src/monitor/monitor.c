#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "monitor.h"

Monitor monitor;

void sigint(int signum)
{
	exit_monitor();
	exit(0);
}

void init_monitor(size_t varcap)
{
	// 创建消息队列
	monitor.msgfd = msgget(MSG_KEY,IPC_CREAT|0644);
	if(0 > monitor.msgfd)
	{
		perror("msgget");
		exit(-1);
	}
	
	// 创建共享内存
	monitor.shmfd = shmget(SHM_KEY,sizeof(Variable)*varcap,IPC_CREAT|0644);
	if(0 > monitor.shmfd)
	{
		perror("shmget");
		exit(-1);
	}
	
	// 映射共享内存
	monitor.arr = shmat(monitor.shmfd,NULL,0);
	if((void *)-1 == monitor.arr)
	{
		perror("shmat");
		exit(-1);
	}
	
	// 初始化所有变量的使用标志位
	for(int i=0; i<varcap; i++)
	{
		monitor.arr[i].usable = false;
	}
	
	// 备份变量的容量
	monitor.varcap = varcap;
	// 初始化变量的数量
	monitor.varcnt = 0;
	
	// 绑定处理信号
	signal(SIGINT,sigint);
}

static int query_null(void)
{
	if(monitor.varcnt >= monitor.varcap)
		return -1;
	
	for(int i=0; i<monitor.varcap; i++)
	{
		if(!monitor.arr[i].usable)
			return i;
	}
	
	return -1;
}

static int query_variable(const char* pname,const char* varname)
{
	printf("monitor.varcnt:%d query_variable:%s | %s\n",monitor.varcnt,pname,varname);
	if(0 == monitor.varcnt)
		return -1;
	
	for(int i=0; i<monitor.varcap; i++)
	{
		if(0 == strcmp(monitor.arr[i].pname,pname) && 
		   0 == strcmp(monitor.arr[i].varname,varname))
			return i;
	}
	
	return -1;
}

void wait_monitored(void)
{
	Msg msg;

	for(;;)
	{
		if(0 > msgrcv(monitor.msgfd,&msg,sizeof(msg)-sizeof(msg.type),0,0))
		{
			continue;
		}
		
		if(Register == msg.type)
		{
			msg.type = RegisterResult;
			msg.id = query_null();
			monitor.arr[msg.id].usable = true;
			monitor.varcnt++;
			msgsnd(monitor.msgfd,&msg,sizeof(msg)-sizeof(msg.type),0);
			
			sleep(1);
			
			char var_log_path[PATH_MAX] = LOG_PATH;
			strcat(var_log_path,monitor.arr[msg.id].varname);
			monitor.arr[msg.id].logfp = fopen(var_log_path,"a+");
		}
		else if(Cancel == msg.type)
		{
			monitor.varcnt--;
			fclose(monitor.arr[msg.id].logfp);
			printf("monitor.varcnt--:%d\n",monitor.varcnt);
		}
	}
}

void wait_viewers(void)
{
	// 创建socket对象
	int sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(0 > sockfd)
	{
		perror("socket");
		return;
	}
	
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(MONITOR_PORT);
	addr.sin_addr.s_addr = inet_addr(MONITOR_IP);
	socklen_t addrlen = sizeof(addr);
	
	if(bind(sockfd,(struct sockaddr*)&addr,addrlen))
	{
		perror("bind");
		return;
	}
	
	char buf[4096];
	size_t buf_size = sizeof(buf);
	
	for(;;)
	{
		recvfrom(sockfd,buf,buf_size,0,
			(struct sockaddr*)&addr,&addrlen);
		printf("recv:%s\n",buf);
			
		int type;
		sscanf(buf,"type:%d",&type);
		
		if(Viewers == type)
		{
			int varid;
			sscanf(buf,"type:%*d data:%d",&varid);
			
			if(varid < 0 || varid >= monitor.varcap)
			{
				sprintf(buf,"type:%d data:-1",ViewersResult);
			}
			else
			{
				sprintf(buf,"type:%d data:%d",ViewersResult,
					monitor.arr[varid].value);
			}	
		}
		else if(Query == type)
		{
			char pname[20],varname[20];
			sscanf(buf,"type:%*d pname:%s varname:%s",pname,varname);
			sprintf(buf,"type:%d data:%d",ViewersResult,
					query_variable(pname,varname));
		}
		
		sendto(sockfd,buf,strlen(buf)+1,0,
			(struct sockaddr*)&addr,addrlen);
		printf("send:%s\n",buf);
	}
}

static const char* get_local_time(void)
{
	static char time_str[15];
	
	time_t local_time = time(NULL);
	struct tm* tm = localtime(&local_time);	
	sprintf(time_str,"%04d%02d%02d%02d%02d%02d",
		tm->tm_year+1900,
		tm->tm_mon+1,
		tm->tm_mday,
		tm->tm_hour,
		tm->tm_min,
		tm->tm_sec);
	
	return time_str;
}

void log_monitor(void)
{
	const char* time_str = get_local_time();
	
	for(int i=0; i<monitor.varcap; i++)
	{
		if(monitor.arr[i].usable)
		{
			int ret = fprintf(monitor.arr[i].logfp,"%s %s %s %u\n",
				time_str,
				monitor.arr[i].pname,
				monitor.arr[i].varname,
				monitor.arr[i].value);
			fflush(monitor.arr[i].logfp);
			printf("log %d 个字节!\n",ret);
		}
	}
}

void exit_monitor(void)
{
	// 取消映射共享内存
	shmdt(monitor.arr);
	
	// 删除共享内存
	shmctl(monitor.shmfd,IPC_RMID,NULL);
	
	// 删除消息队列
	msgctl(monitor.msgfd,IPC_RMID,NULL);
}


