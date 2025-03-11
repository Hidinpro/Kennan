#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <monitor.h>

int main(int argc,const char* argv[])
{
	int sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(0 > sockfd)
	{
		perror("socket");
		return -1;
	}	
	
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(MONITOR_PORT);
	addr.sin_addr.s_addr = inet_addr(MONITOR_IP);
	socklen_t addrlen = sizeof(addr);
	
	if(connect(sockfd,(struct sockaddr*)&addr,addrlen))
	{
		perror("connect");
		return -1;
	}
	
	char buf[4096];
	size_t buf_size = sizeof(buf);
	
	char pname[20],varname[20];
	printf("请输入要查看的进程名和变量名:");
	scanf("%s %s",pname,varname);

	sprintf(buf,"type:%d pname:%s varname:%s",Query,pname,varname);
	
	send(sockfd,buf,strlen(buf)+1,0);
	recv(sockfd,buf,buf_size,0);
	
	int varid = -1;
	sscanf(buf,"type:%*d data:%d",&varid);
	
	if(varid < 0)
	{
		printf("没有找到相关的变量，请检查输入的程序名和变量名!\n");
		return 0;
	}
	
	char cmd[256];
	size_t cmd_size = sizeof(cmd);
	sprintf(cmd,"type:%d data:%d",Viewers,varid);
	
	for(;;)
	{
		send(sockfd,cmd,cmd_size,0);
		recv(sockfd,buf,buf_size,0);
		puts(buf);
		sleep(3);
	}
}
