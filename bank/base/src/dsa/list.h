#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdbool.h>

// 链表节点结构体
typedef struct ListNode
{
	void* ptr;
	struct ListNode* prev;
	struct ListNode* next;
}ListNode;


// 链表结构体
typedef struct List
{
	ListNode* head;
	size_t size;
	int (*nodecmp)(const void*,const void*);
}List;

// 创建节点
ListNode* create_ListNode(void* ptr);

// 创建链表
List* create_list(void);

// 销毁链表
void destroy_list(List* list);

// 清理链表所有节点
void clear_list(List* list);

// 头添加
void add_list(List* list,void* ptr);

// 尾添加
void add_tail_list(List* list,void* ptr);

// 判断是否是空表
bool empty_list(List* list);

// 头删除
void* del_list(List* list);

// 尾删除
void* del_tail_list(List* list);

// 指定位置添加
bool inset_list(List* list,int index,void* ptr);

// 指定位置删除
void* del_index_list(List* list,int index);

// 访问指定位置的节点
void* get_list(List* list,int index);

// 设置节点比较函数
void set_cmp_list(List* list,int (*comper)(const void*,const void*));

// 指定值删除
bool del_value_list(List* list,void* ptr);

// 查询
void* query_list(List* list,void* ptr);

// 排序
void sort_list(List* list);

// 遍历
void fput_list(List* list,void (*fput_ptr)(FILE*,const void*),FILE* fp);

#endif//LIST_H
