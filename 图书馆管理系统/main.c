#include "operator_system.h"
#include "admin_system.h"
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
		printf("１、系统管理  ２、操作员管理 3、退出系统\n");//加载主界面
		switch(get_cmd('1','4'))
		{
			case '1':admin(); break; 
			case '2':operator_menu();break;
			case '3':
					save_data(student,sizeof(Student)*student_cnt,"学生.dat");
					save_data(books,sizeof(Book)*bookcnt,"书.dat");
					save_list();
					return 0;
		}
	}
}