#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdbool.h>

typedef struct QueueNode
{
	void* ptr;
	struct QueueNode* next;
}QueueNode;

typedef struct Queue
{
	QueueNode* front;
	QueueNode* rear;
	size_t size;
}Queue;

// 创建节点
QueueNode* create_queue_node(void* ptr);

// 创建队列
Queue* create_queue(void);

// 销毁队列
void destroy_queue(Queue* queue);

// 队列是否空
bool empty_queue(Queue* queue);

// 入队
void push_queue(Queue* queue,void* ptr);

// 出队
void* pop_queue(Queue* queue);

// 查看队头
void* front_queue(Queue* queue);

// 查看队尾
void* rear_queue(Queue* queue);

// 查看元素数量
size_t size_queue(Queue* queue);

#endif//QUEUE_H
