#ifndef ThreadPool_H
#define ThreadPool_H
#include <pthread.h>
#include <queue.h>

// 为客户端服务的业务逻辑入口
typedef void (*Enter)(void* arg);

typedef struct ThreadPool
{
	pthread_mutex_t lock;
	pthread_cond_t empty;
	pthread_cond_t full;
	size_t pthread_cal;
	pthread_t* tids;
	Queue* queue;
	size_t queue_cal;
	Enter enter;
} ThreadPool;

// 创建线程池
ThreadPool* create_threadpool(size_t pthread_cal,size_t queue_cal,Enter enter);

// 启动线程池
void start_threadpool(ThreadPool* pool);

// 添加任务
void push_threadpool(ThreadPool* pool,void* ptr);

// 结束线程池
void exit_threadpool(ThreadPool* pool);

// 销毁线程池
void destroy_threadpool(ThreadPool* pool);

#endif//ThreadPool_H
