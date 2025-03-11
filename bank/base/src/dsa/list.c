#include "list.h"
#include <stdlib.h>
#include <assert.h>

static void _add_list(ListNode* node,ListNode* prev,ListNode* next)
{
	next->prev = node;
	node->next = next;
	node->prev = prev;
	prev->next = node;
}

static void* _del_list(ListNode* node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
	void* ptr = node->ptr;
	free(node);
	return ptr;
}

static ListNode* _index_list(List* list,int index)
{
	if(index < list->size/2)
	{
		ListNode* node = list->head->next;
		for(int i=0; i<index; i++)
			node = node->next;
		return node;
	}
	
	ListNode* node = list->head->prev;
	for(int i=index; i<list->size-1; i++)
		node = node->prev;
	return node;
}

// 创建节点
ListNode* create_node(void* ptr)
{
	ListNode* node = malloc(sizeof(ListNode));
	node->ptr = ptr;
	node->next = node;
	node->prev = node;
	return node;
}

// 创建链表
List* create_list(void)
{
	List* list = malloc(sizeof(List));
	list->head = create_node(NULL);
	list->nodecmp = NULL;
	list->size = 0;
	return list;
}

// 销毁链表
void destroy_list(List* list)
{
	ListNode* node = list->head;
	do{
		ListNode* temp = node;
		node = node->next;
		free(temp);
	}while(node != list->head);
	
	free(list);
}

// 清理链表所有节点
void clear_list(List* list)
{
	ListNode* node = list->head->next;
	do{
		ListNode* temp = node;
		node = node->next;
		free(temp);
	}while(node != list->head);
	
	list->head->next = list->head;
	list->head->prev = list->head;
	list->size = 0;
}

// 头添加
void add_list(List* list,void* ptr)
{
	_add_list(create_node(ptr),list->head,list->head->next);
	list->size++;
}

// 尾添加
void add_tail_list(List* list,void* ptr)
{
	_add_list(create_node(ptr),list->head->prev,list->head);
	list->size++;
}

// 判断是否是空表
bool empty_list(List* list)
{
	return list->head->next == list->head;
}

// 头删除
void* del_list(List* list)
{
	if(empty_list(list))
		return NULL;
	
	list->size--;
	return _del_list(list->head->next);
}

// 尾删除
void* del_tail_list(List* list)
{
	if(empty_list(list))
		return NULL;
	
	list->size--;
	return _del_list(list->head->prev);
}

// 指定位置添加
bool inset_list(List* list,int index,void* ptr)
{
	if(index >= list->size)
		return false;

	ListNode* node = _index_list(list,index);
	_add_list(create_node(ptr),node->prev,node);
	list->size++;
	return true;
}

// 指定位置删除
void* del_index_list(List* list,int index)
{
	if(index >= list->size)
		return NULL;

	list->size--;
	return _del_list(_index_list(list,index));
}

// 设置节点比较函数
void set_cmp_list(List* list,int (*comper)(const void*,const void*))
{
	list->nodecmp = comper;
}

// 指定值删除
bool del_value_list(List* list,void* ptr)
{
	assert(NULL != list->nodecmp);
	
	ListNode* node = list->head->next;
	while(node != list->head)
	{
		if(0 == list->nodecmp(node->ptr,ptr))
		{
			list->size--;
			return _del_list(node);
		}
		node = node->next;
	}
	return NULL;
}

// 访问指定位置的节点
void* get_list(List* list,int index)
{
	if(index >= list->size)
		return NULL;

	return _index_list(list,index)->ptr;
}



// 查询
void* query_list(List* list,void* ptr)
{
	assert(NULL != list->nodecmp);
	
	ListNode* node = list->head->next;
	while(node != list->head)
	{
		if(0 == list->nodecmp(node->ptr,ptr))
			return node->ptr;
		node = node->next;
	}
	
	return NULL;
}

// 排序
void sort_list(List* list)
{
	assert(NULL != list->nodecmp);
	
	bool flag = true;
	for(ListNode* r=list->head->prev; flag && r!=list->head->next; r=r->prev)
	{
		flag = false;
		for(ListNode* l=list->head->next; l!=r; l=l->next)
		{
			if(1 == list->nodecmp(l->ptr,l->next->ptr))
			{
				void* tmp = l->ptr;
				l->ptr = l->next->ptr;
				l->next->ptr = tmp;
				flag = true;
			}
		}
	}
}

// 遍历
void fput_list(List* list,void (*fput_ptr)(FILE*,const void*),FILE* fp)
{
	ListNode* node = list->head->next;
	while(node != list->head)
	{
		fput_ptr(fp,node->ptr);
		node = node->next;
	}
}
