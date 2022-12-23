#include <stdlib.h>
#include "queue.h"

// 创建节点
QueueNode* create_queue_node(void* ptr)
{
	QueueNode* node = malloc(sizeof(QueueNode));
	node->ptr = ptr;
	node->next = NULL;
	return node;
}

// 创建队列
Queue* create_queue(void)
{
	Queue* queue = malloc(sizeof(Queue));
	queue->front = NULL;
	queue->rear = NULL;
	queue->size = 0;
	return queue;
}

// 销毁队列
void destroy_queue(Queue* queue)
{
	while(!empty_queue(queue))
	{
		pop_queue(queue);
	}
	free(queue);
}

// 队列是否空
bool empty_queue(Queue* queue)
{
	return NULL == queue->front;
}

// 入队
void push_queue(Queue* queue,void* ptr)
{
	QueueNode* node = create_queue_node(ptr);
	if(empty_queue(queue))
	{
		queue->front = node;
		queue->rear = node;
	}
	else
	{
		queue->rear->next = node;
		queue->rear = node;
	}
	queue->size++;
}

// 出队
void* pop_queue(Queue* queue)
{
	if(empty_queue(queue))
		return NULL;

	QueueNode* temp = queue->front;
	queue->front = temp->next;
	queue->size--;

	void* ptr = temp->ptr;
	free(temp);

	return ptr;
}

// 查看队头
void* front_queue(Queue* queue)
{
	return queue->front->ptr;
}

// 查看队尾
void* rear_queue(Queue* queue)
{
	return queue->rear->ptr;
}

// 查看元素数量
size_t size_queue(Queue* queue)
{
	return queue->size;
}
