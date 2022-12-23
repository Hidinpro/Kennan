#include "query.h"

int order_query(void* base,size_t nmemb,size_t size,
	int (*compar)(const void *, const void *),void* ptr)
{
	for(int i=0; i<nmemb; i++)
	{
		if(0 == compar(base+i*size,ptr))
			return i;
	}

	return -1;
}

int binary_query(void* base,size_t nmemb,size_t size,
 	int (*compar)(const void *, const void *),void* ptr)
{
	int l = 0 , r = nmemb;
	while(l < r)
	{
		int p = (l + r) / 2; 
		int result = compar(base+p*size,ptr);
		if(0 == result)
			return p;
		if(1 == result)
			r = p;
		else
			l = p+1;
	}

	return -1;
}
