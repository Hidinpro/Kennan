#include <stdlib.h>
#include <string.h>
#include "tree.h"

typedef struct Book
{
	char isbn[14];
	char title[64];
	char author[20];
	float pirce;
}Book;

int bookcmp(const void* ptr1,const void* ptr2)
{
	return strcmp(((Book*)ptr1)->isbn,((Book*)ptr2)->isbn);
}

void bookput(FILE* fp,const void* ptr)
{
	if(NULL == ptr)
	{
		fprintf(fp,"#\n");
		return;
	}

	Book* book = (Book*)ptr;
	fprintf(fp,"%s %s %s %g\n",book->isbn,book->title,book->author,book->pirce);
}

int main(int argc,const char* argv[])
{
	Tree* tree = create_tree(bookcmp);

	/*
	for(int i=0; i<10; i++)
	{
		Book* book = malloc(sizeof(Book));
		sprintf(book->isbn,"98773200236%02d",rand()%100);
		sprintf(book->title,"哈利波特 巻%d",i);
		sprintf(book->author,"J.K罗琳");
		book->pirce = rand() % 100;
		add_tree(tree,book);
	}

	FILE* fp = fopen("book.txt","w");
	fput_tree(tree,bookput,fp);
	*/

	FILE* rfp = fopen("book.txt","r");
	if(NULL == rfp)
	{
		perror("fopen");
		return 0;
	}

	size_t len = 0;
	void* arr[4096];
	char buf[256];
	while(NULL != fgets(buf,sizeof(buf),rfp))
	{
		if('#' == buf[0])
			arr[len++] = NULL;
		else
		{
			Book* book = malloc(sizeof(Book));
			sscanf(buf,"%s %s %s %f",book->isbn,book->title,book->author,&book->pirce);
			arr[len++] = book;
		}
	}
	init_tree(tree,arr,len);
	fput_tree(tree,bookput,stdout);	

	Book book = {"9877320023677"};
	
	void* ptr = del_tree(tree,&book);
	free(ptr);

	printf("==========================\n");
	fput_tree(tree,bookput,stdout);	
}
