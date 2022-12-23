#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <tools.h>
#include <tcp_socket.h>
#include <pthread.h>
#include "bank.h"


void* run(void* arg)
{
	TcpSocket* cts = (TcpSocket*)arg;
	Msgctos* ctos = malloc(sizeof(Msgctos));
	Msgstoc* stoc = malloc(sizeof(Msgstoc));

	for(;;)
	{
		recv_tcp(cts, ctos, sizeof(*ctos));
		switch(ctos->type)
		{
			case M_CREATE: create_acc(stoc,ctos);break;
			case M_DESTROY:
			case M_LOGIN:
			case M_MODIFY:
			default:
				stoc->type = -1;
				sprintf(stoc->str, "未知的操作！\n");
		}

		send_tcp(cts, stoc, sizeof(*stoc));
	}
}

int main(int argc,const char* argv[])
{
	if(3 != argc)
	{
		puts("Use:./server <ip> <port>");
		return EXIT_SUCCESS;
	}
	TcpSocket* ts = create_tcp(argv[1], atoi(argv[2]));
	listen_tcp(ts);

	for(;;)
	{
		TcpSocket* cts = accept_tcp(ts);
		if(NULL == cts)
		{
			pthread_exit(NULL);
		}

		pthread_t tid;
		pthread_create(&tid, NULL, run, cts);
	}
}
