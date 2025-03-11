#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H
#include <sys/types.h>
#include <sys/ipc.h>

// 创建消息队列
int create_msg(key_t key);

// 获取消息队列
int get_msg(key_t key);

// 发送二进制数据
void send_msg(int msgfd,void* msg,size_t size);

// 发送字符串数据
void send_str_msg(int msgfd,void* msg);

// 接收数据
void recv_msg(int msgfd,void* msg,size_t size);

// 删除消息队列
void remove_msg(int msgfd);

#endif//MSG_QUEUE_H
