#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "udp_socket.h"

typedef struct sockaddr* SAP;

// 修建socket对象，初始化通信地址
UdpSocket* create_udp(const char* ip,short port)
{
	assert(NULL != ip);

	// 给TcpSocket对象分配内在
	UdpSocket* us = malloc(sizeof(UdpSocket));
	if(NULL == us)
	{
		perror("malloc");
		return NULL;
	}

	// 创建socket对象
	us->sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(0 > us->sockfd)
	{
		perror("socket");
		free(us);
		return NULL;
	}

	// 准备通信地址
	us->addr.sin_family = AF_INET;
	us->addr.sin_port = htons(port);
	us->addr.sin_addr.s_addr = inet_addr(ip);
	us->addrlen = sizeof(us->addr);

	return us;
}

// 绑定socket对象与通信地址，服务端专用
int bind_udp(UdpSocket* us)
{
	assert(NULL != us);

	if(bind(us->sockfd,(SAP)&us->addr,us->addrlen))
	{
		perror("bind");
		return -1;
	}
	
	return 0;
}

// 连接服务端，客户端专用
int connect_udp(UdpSocket* us)
{
	assert(NULL != us);

	if(connect(us->sockfd,(SAP)&us->addr,us->addrlen))
	{
		perror("connect");
		return -1;
	}
	return 0;
}

// 发送报文
int send_udp(UdpSocket* us,const void* buf,uint32_t size)
{
	return sendto(us->sockfd,buf,size,0,(SAP)&us->addr,us->addrlen);
}

// 接收报文
int recv_udp(UdpSocket* us,void* buf,uint32_t size)
{
	return recvfrom(us->sockfd,buf,size,0,(SAP)&us->addr,&us->addrlen);
}

// 发送字符串
int send_str_udp(UdpSocket* us,const char* str)
{
	return sendto(us->sockfd,str,strlen(str)+1,0,(SAP)&us->addr,us->addrlen);
}


// 发送报文，调用者自己提供接收者的地址
int send_addr_udp(UdpSocket* us,
				  const void* buf,
				  uint32_t size,
				  struct sockaddr_in* addr)
{
	return sendto(us->sockfd,buf,size,0,(SAP)addr,us->addrlen);
}

// 接收报文，调用者自己提供存储发送地址的内存
int recv_addr_udp(UdpSocket* us,
				  void* buf,
				  uint32_t size,
				  struct sockaddr_in* addr)
{
	return recvfrom(us->sockfd,buf,size,0,(SAP)addr,&us->addrlen);
}

// 关闭socket对象
void close_udp(UdpSocket* us)
{
	assert(NULL != us);
	close(us->sockfd);
	free(us);
}
