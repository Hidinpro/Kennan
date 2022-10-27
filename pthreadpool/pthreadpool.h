#ifndef PTHREADPOOL_H
#define PTHREADPOOL_H
#include <pthread.h>
#include <queue.h>

// 为客户端服务的业务逻辑入口
typedef void (*Enter)(void* arg);

typedef struct Pthreadpool
{
	pthread_mutex_t lock;
	pthread_cond_t empty;
	pthread_cond_t full;
	size_t pthread_cal;
	pthread_t* tids;
	Queue* queue;
	size_t queue_cal;
	Enter enter;
} Pthreadpool;

// 创建线程池
Pthreadpool* create_pthreadpool(size_t pthread_cal,size_t queue_cal,Enter enter);

// 启动线程池
void start_pthreadpool(Pthreadpool* pool);

// 添加任务
void push_pthreadpool(Pthreadpool* pool,int sockfd);

#endif//PTHREADPOOL_H
