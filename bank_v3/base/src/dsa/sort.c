#include <string.h>
#include <stdbool.h>
#include "sort.h"

static void swap_mem(void* p1,void* p2,size_t size)
{
	char tmp[size];
	memcpy(tmp,p1,size);
	memcpy(p1,p2,size);
	memcpy(p2,tmp,size);
}

// 冒泡
void bubble_sort(void* base,size_t nmemb,size_t size,
	int (*compar)(const void *, const void *))
{
	bool flag = true;
	for(int i=nmemb-1; flag && i>0; i--)
	{
		flag = false;
		for(int j=0; j<i; j++)
		{
			if(1 == compar(base+j*size,base+(j+1)*size))
			{
				swap_mem(base+j*size,base+(j+1)*size,size);
				flag = true;
			}
		}
	}
}

// 插入
void insert_sort(void* base,size_t nmemb,size_t size,
	int (*compar)(const void *, const void *))
{
	char tmp[size];
	for(int i=1,j; i<nmemb; i++)
	{
		memcpy(tmp,base+i*size,size);
		for(j=i; j-1>=0 && 1==compar(base+(j-1)*size,tmp); j--)
		{
			memcpy(base+j*size,base+(j-1)*size,size);
		}
		memcpy(base+j*size,tmp,size);
	}
}

// 选择
void select_sort(void* base,size_t nmemb,size_t size,
	int (*compar)(const void *, const void *))
{
	for(int i=0; i<nmemb-1; i++)
	{
		int min = i;
		for(int j=i+1; j<nmemb; j++)
		{
			if(1 == compar(base+min*size,base+j*size))
				min = j;
		}
		if(min != i)
			swap_mem(base+min*size,base+i*size,size);
	}
}

// 快速
void _quick_sort(void* base,int left,int right,size_t size,
	int (*compar)(const void *, const void *))
{
	char pv[size];
	int l = left, r = right , pi = left;
	memcpy(pv,base+pi*size,size);

	while(l<r)
	{
		while(l<r && 1==compar(base+r*size,pv)) r--;
		if(l<r)
		{
			memcpy(base+pi*size,base+r*size,size);
			pi = r;
		}

		while(l<r && 1 > compar(base+l*size,pv)) l++;
		if(l<r)
		{
			memcpy(base+pi*size,base+l*size,size);
			pi = l;
		}
	}
	memcpy(base+pi*size,pv,size);
	if(pi-left > 1)
		_quick_sort(base,left,pi-1,size,compar);
	if(right-pi > 1)
		_quick_sort(base,pi+1,right,size,compar);
}

void quick_sort(void* base,size_t nmemb,size_t size,
	int (*compar)(const void *, const void *))
{
	_quick_sort(base,0,nmemb-1,size,compar);
}
