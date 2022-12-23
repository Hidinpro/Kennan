#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdbool.h>

/*
遇到的问题:
	1、元素出栈时没有返回元素的地址，可能会导致内在泄漏。
	2、对出栈元素数量维的不及时，导致获取的元素数量错误
*/


typedef struct StackNode
{
	void* ptr;
	struct StackNode* next;
}StackNode;

typedef struct Stack
{
	StackNode* top;
	size_t size;
}Stack;

// 创建节点
StackNode* create_stack_node(void* ptr);

// 创建栈
Stack* create_stack(void);

// 销毁栈
void destroy_stack(Stack* stack);

// 栈空
bool empty_stack(Stack* stack);

// 入栈
void push_stack(Stack* stack,void* ptr);

// 出栈
void* pop_stack(Stack* stack);

// 获取栈顶
void* top_stack(Stack* stack);

// 栈元素数量
size_t size_stack(Stack* stack);

#endif//STACK_H
