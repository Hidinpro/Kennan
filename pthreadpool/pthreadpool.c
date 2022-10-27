#include <stdlib.h>
#include "pthreadpool.h"

// 创建线程池
Pthreadpool* create_pthreadpool(size_t pthread_cal,size_t queue_cal,Enter enter)
{
	// 创建线程池对象
	Pthreadpool* pool = malloc(sizeof(Pthreadpool));
	// 初始化锁
	pthread_mutex_init(&pool->lock,NULL);

	// 初始化条件变量 
	pthread_cond_init(&pool->empty,NULL);
	pthread_cond_init(&pool->full,NULL);

	// 分配存储线程ID的空间
	pool->tids = malloc(sizeof(pthread_t)*pthread_cal);
	
	// 备份线程的容量
	pool->pthread_cal = pthread_cal;

	// 创建任务队列
	pool->queue = create_queue();

	// 备份任务队列的容量
	pool->queue_cal = queue_cal;

	// 备份业务逻辑处理入口
	pool->enter = enter;

	// 返回线程池对象
	return pool;
}

// 线程入口函数
static void* run(void* arg)
{
	Pthreadpool* pool = (Pthreadpool*)arg;
	for(;;)
	{
		pthread_mutex_lock(&pool->lock);
	
		while(empty_queue(pool->queue))
		{
			pthread_cond_broadcast(&pool->full);
			pthread_cond_wait(&pool->empty,&pool->lock);
		}
	
		int* ptr = pop_queue(pool->queue);

		pthread_mutex_unlock(&pool->lock);
	
		pool->enter(ptr);
	}
}

// 启动线程池
void start_pthreadpool(Pthreadpool* pool)
{
	for(int i=0; i<pool->pthread_cal; i++)
	{
		pthread_create(pool->tids+i,NULL,run,pool);
	}
}

// 添加任务
void push_pthreadpool(Pthreadpool* pool,int sockfd)
{
	pthread_mutex_lock(&pool->lock);
	while(pool->queue_cal <= size_queue(pool->queue))
	{
		pthread_cond_broadcast(&pool->empty);
		pthread_cond_wait(&pool->full,&pool->lock);
	}

	int* ptr = malloc(sizeof(int));
	*ptr = sockfd;

	push_queue(pool->queue,ptr);

	pthread_cond_signal(&pool->empty);

	pthread_mutex_unlock(&pool->lock);
}
