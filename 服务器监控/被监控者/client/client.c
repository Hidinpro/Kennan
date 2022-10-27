#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <tcp_socket.h>

void* run(void* arg)
{
	TcpSocket* cts = (TcpSocket*)arg;

	char buf[4096];
	for(;;)
	{
		if(0 > recv_str_tcp(cts,buf))
			break;
		printf("\r%s\n>>>",buf);
		fflush(stdout);
	}
}

int main(int argc,const char* argv[])
{
	if(3 != argc)
	{
		puts("Use:./client <ip> <port>");
		return 0;
	}

	TcpSocket* cts = create_tcp(argv[1],atoi(argv[2]));
	if(connect_tcp(cts))
		return -1;

	char buf[4096];
	int logon_cnt = 0;
	while(logon_cnt < 3)
	{
		printf("请输入昵称：");
		scanf("%s",buf);

		send_str_tcp(cts,buf);
		recv_str_tcp(cts,buf);
		puts(buf);
		
		if(0 == strncmp("欢迎",buf,6))
			break;
		
		logon_cnt++;
	}

	if(logon_cnt>=3)
	{
		recv_str_tcp(cts,buf);
		printf("recv:%s\n",buf);
	}

	pthread_t tid;
	pthread_create(&tid,NULL,run,cts);

	for(;;)
	{
		printf(">>>");
		scanf("%s",buf);
		send_str_tcp(cts,buf);
		if(0 == strcmp("quit",buf))
			break;
	}
	
	close_tcp(cts);
	return 0;
}
