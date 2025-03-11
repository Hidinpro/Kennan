#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <tcp_socket.h>
#include <threadpool.h>
#include <list.h>
#include <monitored.h>
#define PTHREAD_CAL	5

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

// 链表的回调函数，用于判断昵称是否重名
int namecmp(const void* p1,const void* p2)
{
	return strcmp(((Client*)p1)->name,(char*)p2);
}

// 客户端注册昵称
Client* logon(TcpSocket* cts)
{
	pthread_mutex_lock(&list_lock);
	
	// 临时存储客户端的昵称
	char name[256];
	
	// 三次注册机会
	int logon_cnt = 0;
	while(logon_cnt < 3)
	{
		// 接收客户端的昵称
		recv_str_tcp(cts,name);
		// 设置回调函数
		set_cmp_list(client_list,namecmp);
		// 判断是否有重名
		if(NULL == query_list(client_list,name))
			break;

		// 检测到重名
		send_str_tcp(cts,"该名字已经被战用！");
		logon_cnt++;
	}

	// 三次机会用完，服务端主动关闭与客户端的连接
	if(logon_cnt >= 3)
	{
		pthread_mutex_unlock(&list_lock);
		send_str_tcp(cts,"你可真是个大聪明，下次吧！");
		close_tcp(cts);
		return NULL;
	}
	
	// 向客户端发送欢迎信息和上网提醒
	send_str_tcp(cts,"欢迎进入砍一刀聊天室！请文明上网，遵守法律法规！");

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
	// 向所有客户端发送某人的发言
	ListNode* node = client_list->head->next;
	while(node != client_list->head)
	{
		// 排除它自己
		if(client != node->ptr)
			send_str_tcp(((Client*)node->ptr)->cts,buf);
		node = node->next;
	}
	pthread_mutex_unlock(&list_lock);
}

int cleintcmp(const void* p1,const void* p2)
{
	return p1 == p2;
}

void logoff(Client* client,char* buf)
{
	// 向所有客户端发送某人退出的消息
	send_all_client(client,buf);

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
	// 客户端数量加1
	add_monitored(client_cnt_id,1);

	
	// 把客户端的名字存储到缓冲区
	char buf[4096] = {};
	strcat(buf,client->name);
	strcat(buf,":");	
	
	// 计算出该客户端的发言的存储位置，避免了每次拷贝昵称 
	char* start = buf+strlen(buf);
	
	// 通知所有客户端，有人进入
	strcpy(start,"进入聊天室!!!");
	send_all_client(client,buf);

	for(;;)
	{
		// 接收客户端的发言
		int ret = recv_str_tcp(client->cts,start);
		if(ret > 0 || 0 == strcmp("quit",start))
		{
			add_monitored(client_cnt_id,-1);
			logoff(client,buf);
			break;
		}

		// 转发给所有客户端
		send_all_client(client,buf);
	}
	
	// 为该客户端服务结束，回到线程池等待新的客户端
}

void sigint(int signum)
{
	// 结束线程池
	exit_threadpool(pool);

	// 销毁线程池
	destroy_threadpool(pool);
	
	// 向所有客户端发送升级维护信息
	send_all_client(NULL,"服务器：我现在需要升级维护，暂不能提供聊天服务");
	
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

	// 初始化监控模块
	init_monitored(argv[0]);
	printf("注册变量\n");
	// 注册监控变量,获取变量的id 
	client_cnt_id = register_monitored("client_cnt");
	printf("注册变量成功\n");

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
