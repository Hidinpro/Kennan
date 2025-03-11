#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "pthreadpool.h"

void error_exit(const char* func)
{
	printf("%s:%m\n",func);
	exit(EXIT_FAILURE);
}

void enter(void* arg)
{
	int cli_fd = *(int*)arg;
	char buf[4096];
	for(;;)
	{
		// 接收数据
		size_t ret = recv(cli_fd,buf,sizeof(buf),0);
		if(0 >= ret || 0 == strcmp("quit",buf))
			break;

		printf("recv:%s bytes:%u\n",buf,ret);
		strcat(buf,":return");

		// 返回数据
		ret = send(cli_fd,buf,strlen(buf)+1,0);
		if(0 >= ret)
			break;

	}
	printf("通信结束！\n");
	// 关闭通信socket
	close(cli_fd);
}

int main(int argc,const char* argv[])
{
	if(2 != argc)
	{
		printf("Use:./a.out <port>\n");
		return EXIT_FAILURE;
	}
	// 创建socket对象
	int svr_fd = socket(AF_INET,SOCK_STREAM,0);
	if(0 > svr_fd)
		error_exit("socket");

	// 准备通信地址
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[1]));
	addr.sin_addr.s_addr = inet_addr("10.0.2.15");
	socklen_t addrlen = sizeof(addr);

	// 绑定socket对象和通信地址
	if(bind(svr_fd,(struct sockaddr*)&addr,addrlen))
		error_exit("bind");

	// 开启监听
	if(listen(svr_fd,3))
		error_exit("listen");

	// 创建线程池
	Pthreadpool* pool = create_pthreadpool(3,20,enter);
	// 启动线程池
	start_pthreadpool(pool);
		
	for(;;)
	{
		// 等待连接
		int cli_fd = accept(svr_fd,NULL,NULL);
		if(0 > cli_fd)
			error_exit("accept");

		push_pthreadpool(pool,cli_fd);
	}

	// 关闭连接socket
	close(svr_fd);
	return EXIT_SUCCESS;
}
