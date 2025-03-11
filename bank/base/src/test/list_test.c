#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "list.h"

typedef struct BookRecord
{
	char isbn[14];
	char type;
	time_t time;
}BookRecord;

void put_record(FILE* fp,const void* ptr)
{
	BookRecord* recore = (BookRecord*)ptr;
	fprintf(fp,"%s %hhd %lu\n",recore->isbn,recore->type,recore->time);
}

int cmp_recore(const void* ptr1,const void* ptr2)
{
	return strcmp(((BookRecord*)ptr1)->isbn,((BookRecord*)ptr2)->isbn);
}

int main(int argc,const char* argv[])
{
	List* booklist = create_list();
	for(int i=0; i<20; i++)
	{
		BookRecord* record = malloc(sizeof(BookRecord));
		sprintf(record->isbn,"98773200236%02d",rand()%100);
		record->type = 1;
		record->time = time(NULL);
		printf("%d\n",i+1);
		sleep(1);
		add_tail_list(booklist,record);
	}
	
	fput_list(booklist,put_record,stdout);
	set_cmp_list(booklist,cmp_recore);
	sort_list(booklist);
	printf("-----------------------------------\n");
	fput_list(booklist,put_record,stdout);

	BookRecord rocord = {"9877320023640"};
	del_value_list(booklist,&rocord);
	printf("-----------------------------------\n");
	fput_list(booklist,put_record,stdout);

	sprintf(rocord.isbn,"9877320023677");
	BookRecord* br = query_list(booklist,&rocord);
	printf("%lu\n",br->time);

	return 0;
}
