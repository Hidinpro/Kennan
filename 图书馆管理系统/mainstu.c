#include "operator_system.h"
#include "admin_system.h"
#include "student_system.h"
#include <stdlib.h>

int main(int argc,const char* argv[])
{
	student_cnt = load_data(student,sizeof(student),"学生.dat") / sizeof(Student);
	bookcnt = load_data(books,sizeof(books),"书.dat") / sizeof(Book);
	load_list();
	create_index(books, indexs);
	for(;;)
	{
		//system("clear");
		printf("１、学生界面 2、退出系统\n");//加载主界面
		switch(get_cmd('1','2'))
		{
			case '1':enter(); break; 
			case '2':
                    save_data(student,sizeof(Student)*student_cnt,"学生.dat");
					save_data(books,sizeof(Book)*bookcnt,"书.dat");
					save_list();
					return 0;
		}
	}
}