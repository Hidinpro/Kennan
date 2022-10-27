#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H
#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>

typedef struct TcpSocket
{
	int sockfd;
	socklen_t addrlen;
	struct sockaddr_in addr;
}TcpSocket;

// 创建socket对象，并初始化通信地址
TcpSocket* create_tcp(const char* ip,short port);

// 绑定socket对象和通信地址，开启监听
int listen_tcp(TcpSocket* ts);

// 等待客户端连接
TcpSocket* accept_tcp(TcpSocket* ts);

// 连接服务端
int connect_tcp(TcpSocket* ts);

// 发送字符串 
int send_str_tcp(TcpSocket* ts,const char* str);

// 接收一个字符串
int recv_str_tcp(TcpSocket* ts,char* buf);

// 发送一个结构体
int send_tcp(TcpSocket* ts,const void* buf,uint32_t size);

// 接收一个结点体，直到接收size个字节为止
int recv_tcp(TcpSocket* ts,void* buf,uint32_t size);

// 关闭socket，销毁内存
void close_tcp(TcpSocket* ts);

#endif//TCP_SOCKET_H
