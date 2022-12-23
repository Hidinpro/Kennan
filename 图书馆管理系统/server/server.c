#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <tcp_socket.h>
#include <threadpool.h>
#include <list.h>
#include <monitored.h>
#include "add.h"
#define PTHREAD_CAL	5

//	命令结构体，存储命令的名称以及命令函数的指针
struct cmd_t
{
	const char* cmd;
	void (*cmdfp)();
};

//	定义cmd_t 结构体数组，方便下面进行判断
static struct cmd_t cmds[] = {
	{"add_operator",add_operator},

};

// 存储客户端的相关信息
typedef struct Client
{
	char name[20];	// 客户端昵称
	TcpSocket* cts;	// 为客户端服务的socket对象
}Client;

TcpSocket* ts;

// 线程池
ThreadPool* pool;

// 监控客户端数据的变量id
int client_cnt_id;

// 存储客户端信息的链表
List* client_list;
pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;

int cmd_cnt = 1;

// 客户端注册昵称
Client* logon(TcpSocket* cts)
{
	pthread_mutex_lock(&list_lock);
	
	// 创建客户端结构变量 
	Client* client = malloc(sizeof(Client));
	// 记录客户端和昵称和为它服务的socket对象
	strcpy(client->name,name);
	client->cts = cts;

	// 把客户端添加到链表
	add_tail_list(client_list,client);
	pthread_mutex_unlock(&list_lock);
	return client;
}

void send_all_client(Client* client,char* buf)
{

	pthread_mutex_lock(&list_lock);
    //发送消息
	send_str_tcp(((Client*)node->ptr)->cts,buf);

	pthread_mutex_unlock(&list_lock);
}

int cleintcmp(const void* p1,const void* p2)
{
	return p1 == p2;
}

//  删除客户端
void logoff(Client* client,char* buf)
{
	// 关闭socket对象
	close_tcp(client->cts);

	pthread_mutex_lock(&list_lock);
	// 从链表删除客户端
	del_value_list(client_list,client);
	pthread_mutex_unlock(&list_lock);
	
	// 释放客户端结构变量
	free(client);
}

void enter(void* arg)
{
	printf("有客户端连接！\n");
	// 客户端注册 
	Client* client = logon(arg);
	
	if(NULL == client)
	{
		printf("注册失败！\n");
		return;
	}
	
	printf("注册成功！\n");

    char cmd[4096];
	for(;;)
	{
		// 接收客户端的发言
		int ret = recv_str_tcp(client->cts,cmd);

        for(int i=0; i<cmd_cnt; i++)//	匹配相应的命令并调用函数
		{
			int len = strlen(cmds[i].cmd);
			if(0 == strncmp(cmd,cmds[i].cmd,len))
			{
				cmds[i].cmdfp();
				break;
			}
		}	
		if(ret > 0 || 0 == strcmp("quit",cmd))
		{
			logoff(client,buf);
			break;
		}
	}
	
	// 为该客户端服务结束，回到线程池等待新的客户端
}

void sigint(int signum)
{
	// 结束线程池
	exit_threadpool(pool);

	// 销毁线程池
	destroy_threadpool(pool);
	
	// 销毁链表
	destroy_list(client_list);
	
	// 关闭负责连接的socket对象
	close_tcp(ts);
	
	// 注销监控变量 
	cancel_monitored(client_cnt_id);
	
	// 退出监控模块
	exit_monitored();
	
	printf("------------结束-------\n");
	exit(0);
}

int main(int argc,const char* argv[])
{
	if(3 != argc)
	{
		puts("Use:./server <ip> <port>");
		return EXIT_SUCCESS;
	}
	
	// 绑定信号处理函数
	signal(SIGINT,sigint);
	
	printf("--------创建socket-----------\n");
	// 创建socket对象
	ts = create_tcp(argv[1],atoi(argv[2]));
	printf("--------创建socket成功-----------\n");
	// 开启socket的监听
	if(NULL == ts || listen_tcp(ts))
		return EXIT_FAILURE;

	printf("---------监听-----------\n");
	// 创建存储客户端信息的链表
	client_list = create_list();
		
	// 创建线程池
	pool = create_threadpool(PTHREAD_CAL,PTHREAD_CAL,enter);
	
	// 启动线程池，子线程等待任务
	start_threadpool(pool);

	// 主线程负责收客户端的连接并向线程池添加任务
	for(;;)
	{
		printf("等待客户端连接...\n");
		TcpSocket* cts = accept_tcp(ts);
		if(NULL != cts)
		{
			// 向线程池添加任务
			push_threadpool(pool,cts);
		}
	}
	return 0;
}
