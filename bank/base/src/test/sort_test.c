#include <stdio.h>
#include <sort.h>

int main(int argc,const char* argv[])
{
	int doublecmp(const void* p1,const void* p2)
	{
		if(*(double*)p1 > *(double*)p2)
			return 1;
		if(*(double*)p1 < *(double*)p2)
			return -1;
		return 0;
	}

	double arr[10] = {2.1,3.4,3.3,2.1,5.5,3.6,8.9,0.9,6.6,3.14};
	quick_sort(arr,10,8,doublecmp);
	for(int i=0; i<10; i++)
	{
		printf("%g ",arr[i]);
	}

}
