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
    {
        printf("gg\n");
		return -1;
    }

	pthread_t tid;
	pthread_create(&tid,NULL,run,cts);

	for(;;)
	{
        add_operator(cts);
	}
	
	close_tcp(cts);
	return 0;
}
