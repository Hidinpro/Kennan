#include <stdio.h>
#include "tools.h"

int main()
{
	typedef struct Student
	{
		char name[20];
		int id;
		float score;
	}Student;

	int put_stu(const void* ptr,FILE* fp)
	{
		const Student* stu = ptr;
		return fprintf(fp,"%s %d %f\n",stu->name,stu->id,stu->score);
	}

	int get_stu(void* ptr,FILE* fp)
	{
		Student* stu = ptr;
		return fscanf(fp,"%s %d %f\n",stu->name,&stu->id,&stu->score);
	}

	/*
	Student stus[] = {
		{"hehe1",10011,91},
		{"hehe2",10011,91},
		{"hehe3",10011,91},
		{"hehe4",10011,91},
		{"hehe5",10011,91},
		{"hehe6",10011,91},
	};

	int ret = save_text(stus,sizeof(stus[0]),ARR_LEN(stus),"stu.txt",put_stu);
	printf("stu cnt %d\n",ret);
	*/
	Student* stup = my_malloc(sizeof(Student)*10);
	int cnt = load_text(stup,sizeof(Student),10,"stu.txt",get_stu);

	for(int i=0; i<cnt; i++)
	{
		put_stu(stup+i,stdout);
	}
	my_free(stup);

}
