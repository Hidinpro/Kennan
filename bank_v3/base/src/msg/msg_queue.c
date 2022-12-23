#include "msg_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/msg.h>

// 创建消息队列
int create_msg(key_t key)
{
	int msgfd = msgget(key,IPC_CREAT/*|IPC_EXCL*/|0600);
	if(0 >msgfd)
	{
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	return msgfd;
}

// 获取消息队列
int get_msg(key_t key)
{
	int msgfd = msgget(key,0);
	if(0 >msgfd)
	{
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	return msgfd;
}

// 发送二进制数据
void send_msg(int msgfd,void* msg,size_t size)
{
	if(msgsnd(msgfd,msg,size-sizeof(long),0))
	{
		perror("msgsnd data");
	}
}

// 发送字符串数据
void send_str_msg(int msgfd,void* msg)
{
	if(msgsnd(msgfd,msg,strlen(msg+sizeof(long))+1,0))
	{
		perror("msgsnd data");
	}
}

// 接收数据
void recv_msg(int msgfd,void* msg,size_t size)
{
	if(-1 == msgrcv(msgfd,msg,size-sizeof(long),*(long*)msg,0))
	{
		perror("msgrcv");
	}
}

// 删除消息队列
void remove_msg(int msgfd)
{
	if(msgctl(msgfd,IPC_RMID,NULL))
	{
		perror("msgctl");
	}
}
