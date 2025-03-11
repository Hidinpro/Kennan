#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tcp_socket.h"

typedef struct sockaddr* SAP;

// 创建socket对象，并初始化通信地址
TcpSocket* create_tcp(const char* ip,short port)
{
	assert(NULL != ip);

	// 给TcpSocket对象分配内在
	TcpSocket* ts = malloc(sizeof(TcpSocket));
	if(NULL == ts)
	{
		perror("malloc");
		return NULL;
	}

	// 创建socket对象
	ts->sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(0 > ts->sockfd)
	{
		perror("socket");
		free(ts);
		return NULL;
	}

	// 准备通信地址
	ts->addr.sin_family = AF_INET;
	ts->addr.sin_port = htons(port);
	ts->addr.sin_addr.s_addr = inet_addr(ip);
	ts->addrlen = sizeof(ts->addr);

	return ts;
}

// 绑定socket对象和通信地址，开启监听
int listen_tcp(TcpSocket* ts)
{
	assert(NULL != ts);

	if(bind(ts->sockfd,(SAP)&ts->addr,ts->addrlen))
	{
		perror("bind");
		return -1;
	}

	if(listen(ts->sockfd,3))
	{
		perror("listen");
		return -1;
	}

	return 0;
}

// 等待客户端连接
TcpSocket* accept_tcp(TcpSocket* ts)
{
	assert(NULL != ts);

	TcpSocket* cts = malloc(sizeof(TcpSocket));
	if(NULL == cts)
	{
		perror("malloc");
		return NULL;
	}

	cts->addrlen = sizeof(cts->addr);
	cts->sockfd = accept(ts->sockfd,(SAP)&cts->addr,&cts->addrlen);
	if(0 > cts->sockfd)
	{
		perror("accept");
		free(cts);
		return NULL;
	}

	return cts;
}

// 连接服务端
int connect_tcp(TcpSocket* ts)
{
	assert(NULL != ts);

	if(connect(ts->sockfd,(SAP)&ts->addr,ts->addrlen))
	{
		perror("connect");
		return -1;
	}
	return 0;
}

// 发送字符串 使用\n作为字符串的结束标志
int send_str_tcp(TcpSocket* ts,const char* str)
{
	assert(NULL != ts && NULL != str);
	return send(ts->sockfd,str,strlen(str)+1,0);
}

// 发送一个结构体
int send_tcp(TcpSocket* ts,const void* buf,uint32_t size)
{
	assert(NULL != ts && NULL != buf);
	return send(ts->sockfd,buf,size,0);
}

// 接收一个字符串，直到接收到\n为止
int recv_str_tcp(TcpSocket* ts,char* str)
{
	assert(NULL != ts && NULL != str);
	char* tmp = str;
	do{
		if(0 >= recv(ts->sockfd,tmp,1,0))
		{
			perror("recv_str_tcp");
			return -1;
		}
	}while(*tmp++);

	return tmp-str-1;
}

// 接收一个结点体，直到接收size个字节为止
int recv_tcp(TcpSocket* ts,void* buf,uint32_t size)
{
	assert(NULL != ts && NULL != buf);
	while(size)
	{
		int ret = recv(ts->sockfd,buf,size,0);
		if(0 >= ret)
		{
			perror("recv_data_tcp");
			return -1;
		}

		buf += ret;
		size -= ret;
	}

	return 0;
}

// 关闭socket，销毁内存
void close_tcp(TcpSocket* ts)
{
	assert(NULL != ts);
	close(ts->sockfd);
	free(ts);
}
