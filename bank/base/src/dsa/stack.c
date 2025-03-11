#include <stdlib.h>
#include "stack.h"

// 创建节点
StackNode* create_stack_node(void* ptr)
{
	StackNode* node = malloc(sizeof(StackNode));
	node->ptr = ptr;
	node->next = NULL;
	return node;
}

// 创建栈
Stack* create_stack(void)
{
	Stack* stack = malloc(sizeof(Stack));
	stack->top = NULL;
	stack->size = 0;
	return stack;
}

// 销毁栈
void destroy_stack(Stack* stack)
{
	while(!empty_stack(stack))
	{
		pop_stack(stack);
	}
	free(stack);
}

// 栈空
bool empty_stack(Stack* stack)
{
	return NULL == stack->top;
}

// 入栈
void push_stack(Stack* stack,void* ptr)
{
	StackNode* node = create_stack_node(ptr);
	node->next = stack->top;
	stack->top = node;
	stack->size++;
}

// 出栈
void* pop_stack(Stack* stack)
{
	if(empty_stack(stack))
		return NULL;

	StackNode* temp = stack->top;
	stack->top = temp->next;
	stack->size--;

	void* ptr = temp->ptr;
	free(temp);
	
	return ptr;
}

// 获取栈顶
void* top_stack(Stack* stack)
{
	return stack->top->ptr;
}

// 栈元素数量
size_t size_stack(Stack* stack)
{
	return stack->size;
}
