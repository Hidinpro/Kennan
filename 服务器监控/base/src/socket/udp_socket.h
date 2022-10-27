#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H
#include <netinet/in.h>
#include <stdint.h>

typedef struct UdpSocket
{
	int sockfd;
	socklen_t addrlen;
	struct sockaddr_in addr;
}UdpSocket;

// 修建socket对象，初始化通信地址
UdpSocket* create_udp(const char* ip,short port);

// 绑定socket对象与通信地址，服务端专用
int bind_udp(UdpSocket* us);

// 连接服务端，客户端专用
int connect_udp(UdpSocket* us);

// 发送报文
int send_udp(UdpSocket* us,const void* buf,uint32_t size);

// 接收报文
int recv_udp(UdpSocket* us,void* buf,uint32_t size);

// 发送字符串
int send_str_udp(UdpSocket* us,const char* buf);


// 发送报文，调用者自己提供接收者的地址
int send_addr_udp(UdpSocket* us,
				  const void* buf,
				  uint32_t size,
				  struct sockaddr_in* addr);

// 接收报文，调用者自己提供存储发送地址的内存
int recv_addr_udp(UdpSocket* us,
				  void* buf,
				  uint32_t size,
				  struct sockaddr_in* addr);

// 关闭socket对象
void close_udp(UdpSocket* us);

#endif//UDP_SOCKET_H
