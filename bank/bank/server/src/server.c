#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <msg_queue.h>
#include <tools.h>
#include "bank.h"
#include <netinet/in.h>
#include <arpa/inet.h>

// 子进程数量
#define CHILD_CNT 5

// 子进程可执行文件
static char* child_process[] = {
	"create",
	"destroy",
	"login",
	"transf",
	"modify",
	"unlock",
	"save",
	"take",
	"query",
};
	
// 存储子进程ID
static pid_t child_pid[CHILD_CNT];

// 消息队列描述符
int msgfd;
int svr_fd;
int cli_fd;
void sigfunc(int signum)
{
	put_msg("主进程收到异常信号!\n");
	for(int i=0; i<CHILD_CNT; i++)
	{
		put_msg("主进程向子进程%s发送SIGINT信号!\n",child_process[i]);
		kill(child_pid[i],SIGINT);
	}

	put_msg("主进程删除消息队列！\n");
	remove_msg(msgfd);
	close(svr_fd);
	close(cli_fd);
	put_msg("主进程退出！\n");
	exit(EXIT_SUCCESS);
}

void error_exit(const char* func)
{
	printf("%s:%m\n",func);
	exit(EXIT_FAILURE);
}

int main(int argc,const char* argv[])
{

#ifndef DEBUG
	int log_fd = open(LOG_PATH,O_WRONLY|O_CREAT,0644);
	if(0 > log_fd)
	{
		put_msg("打开日志文件失败，open:%m\n");
		return EXIT_FAILURE;
	}
	
	dup2(log_fd,STDIN_FILENO);
#endif

	put_msg("绑定SOCKET对象\n");
	for(int sig=SIGHUP; sig<=SIGRTMAX; sig++)
	{
		signal(sig,sigfunc);
	}
	
	//创建socket对象
	svr_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > svr_fd)
	{
		error_exit("socket");
	}

	//准备通信地址
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(2124);
	addr.sin_addr.s_addr = inet_addr("10.0.2.15");
	socklen_t addrlen = sizeof(addr);
	//绑定socket对象和通信地址
	if(bind(svr_fd, (struct sockaddr*)&addr, addrlen))
	{
		error_exit("bind");
	}
	if(listen(svr_fd,3))
	{
		error_exit("listen");
	}
	//等待连接

	cli_fd = accept(svr_fd, NULL, NULL);
	if(0 > cli_fd)
	{
		error_exit("accept");
	}
	char cli_fdc[20]; 
	sprintf(cli_fdc,"%d",cli_fd);

	for(int i=0; i<CHILD_CNT; i++)
	{
		child_pid[i] = vfork();
		if(0 == child_pid[i])
		{
			execl(child_process[i],child_process[i],cli_fdc, NULL);
		}
		put_msg("加载子进程：%s 进程号:%u\n",child_process[i],child_pid[i]);
	}
	int status;
	pid_t pid;
	while(-1 != (pid = wait(&status)))
	{
		put_msg("子进程：%u，exit status is %d\n",pid,WEXITSTATUS(status));
	}
	return 0;
}
