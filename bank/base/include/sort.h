#ifndef SORT_H
#define SORT_H
#include <stdio.h>

// 冒泡
void bubble_sort(void* base,size_t nmemb,size_t size,
	int (*compar)(const void *, const void *));

// 插入
void insert_sort(void* base,size_t nmemb,size_t size,
	int (*compar)(const void *, const void *));

// 选择
void select_sort(void* base,size_t nmemb,size_t size,
	int (*compar)(const void *, const void *));

// 快速
void quick_sort(void* base,size_t nmemb,size_t size,
	int (*compar)(const void *, const void *));

#endif//SORT_H
