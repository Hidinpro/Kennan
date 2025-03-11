#ifndef QUERY_H
#define QUERY_H
#include <stdio.h>

int order_query(void* base,size_t nmemb,size_t size,
	int (*compar)(const void *, const void *),void* ptr);

int binary_query(void* base,size_t nmemb,size_t size,
 	int (*compar)(const void *, const void *),void* ptr);

#endif//QUERY_H
