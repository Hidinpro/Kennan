#include "admin_system.h"
#include "tools.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <getch.h>



Operator operator[OPERATOR_MAX];
int operator_cnt;


int query_operatorid(const int id) 		//用操作员ID 查找操作员
{
	for(int i=0; i<operator_cnt; i++)
	{
		if(id == operator[i].id)
			return i;
	}
	return -1;
}

int get_personid(void)          		//自动寻找后勤空余ID
{
	for(int i=101;i<200;i++)
	{
		if(-1 == query_operatorid(i))
		{
			return i;
		}
	}
	return -1;
}

int query_studentid(char* id) 		//用学生ID 查找学生
{
	for(int i=0; i<student_cnt; i++)
	{
		if(!strcmp(id,student[i].student_id))
			return i;
	}
	return -1;
}

//菜单

void menu(void)
{
	system("clear");
    puts("---------------------------------------------------------\n");
    puts("******图书管管理系统******");
	puts("1、系统管理员登录");
	puts("2、退出");
    puts("---------------------------------------------------------\n");
}



void admin_menu(void)               
{
    system("clear");
    puts("---------------------------------------------------------\n");
    puts("******图书管管理系统******");
	puts("系统管理员");
	puts("1、操作员管理");
	puts("2、学生管理");
	puts("3、返回");
    puts("---------------------------------------------------------\n");
}

//管理员系统

int book_admin(void)  //管理员登录
{
system("clear");

	printf("请输入管理员名称\n");
	char user[20];
	get_str(user,sizeof(user));

	printf("请输入密码\n");
	char password[7];
	get_passwd(password,sizeof(password));
	if(0 == strcmp(user,"admin") && 0 == strcmp(password,"123456"))
	{
		show_msg("登录成功",TIME);
		return 0;
	}
	show_msg("用户名或密码错误!",TIME);
	return -1;	
}

void admin(void)                        //管理员界面
{
	if(0 != book_admin())
	{
		return ;
	}
    for(;;)
	{
		admin_menu();

		switch(get_cmd('1','3'))
		{
		case '1': register_operator();break;       //管理操作员
		case '2': register_student();break;      //管理学生
		case '3': return;
		}
	}
}


void admin_operator_menu(void)     //管理员管理操作员菜单
{
    system("clear");
    puts("---------------------------------------------------------\n");
    puts("******操作员系统******");
	puts("1、增加操作员");
	puts("2、开除操作员");
	puts("3、修改操作员");
    puts("4、查看操作员");
	puts("5、返回上一级");
    puts("---------------------------------------------------------\n");
}
int login_operator(void)     //操作员ID
{
	int id;
	// 输入操作员ID
	system("clear");
    printf("请输入ID：");
	scanf("%d",&id);
	getchar();

	// 查询操作员ID是否存在
	int index = query_operatorid(id);
	if(-1 == index)
	{
		show_msg("该ID不存在，请检查ID是否正确！",TIME);
		return -1;
	}
	printf("请输入操作员密码\n");
	char password[7];
	get_passwd(password,sizeof(password));
	if(0 == strcmp(operator[index].password,password))
	{
		show_msg("登录成功!",TIME);
		return 0;
	}
	show_msg("密码错误!",TIME);
	return -1;
}

void register_operator(void)           //管理操作员界面
{
	
	for(;;)
	{
		admin_operator_menu();

		switch(get_cmd('1','5'))
		{
		case '1': add_operator();break;        //增加操作员
		case '2': reduce_operator();break;     //开除操作员
		case '3': change_operator(); ;break;   //修改操作员密码
        case '4': show_operator();break;       //查询操作员的信息
		case '5': return;
		}
	}
}

void add_operator(void)   			//增加操作员
{

    // 判断用户是否已满
	if(operator_cnt >= OPERATOR_MAX)
	{
		show_msg("后勤人员数量已满，无法添加！",TIME);
		return;
	}

	// 检查ID是否被占用
    if(-1 < query_operatorid(operator[operator_cnt].id))
    {
	show_msg("该学号已被占用，无法注册！",TIME);
	return; 
    }

	//输入ID
	operator[operator_cnt].id = get_personid();//自动获取空余ID函数;
	printf("后勤人员ID为:%d\n",operator[operator_cnt].id);


	// 输入密码
	char password[7];
	char repassword[7];
	printf("请输入密码:\n");
	get_passwd(password,sizeof(password));
	printf("请再次输入密码:\n");
	get_passwd(repassword,sizeof(repassword));
	
	if(strcmp(password,repassword))			
	{
		show_msg("两次密码输入不一,注册失败!",TIME);
		return;
	}

	strcpy(operator[operator_cnt].password,password);

    operator_cnt++;

	show_msg("注册成功!",TIME);
}



void reduce_operator(void)   		//选择输入操作员ID来进行删除
{
	system("clear");
	printf("请输入后勤员工ID:\n");

	int id;
	scanf("%d",&id);

	int place = query_operatorid(id);
	if(-1 == place)
	{
		show_msg("没有该ID的操作员!\n",TIME);
		return;
	}
	if(yes_or_no())
	{
			// 把最后一个用户移动给要删除的用户位置
			operator[place] = operator[operator_cnt-1];
			// 操作员人数数量减1
			operator_cnt--;
			show_msg("删除操作员成功！",TIME);
	}
	else
	{
		    show_msg("取消删除操作！",TIME);
	}
		
}

void change_operator(void)                   //修改操作员密码
{
	system("clear");
	printf("请输入操作员ID:\n");

	int id;
	scanf("%d",&id);
    getch();
	int position = query_operatorid(id);
	if(-1 == position)
	{
		show_msg("没有该ID的员工!\n",TIME);
		return;
	}
	 
    char password[7];
	char repassword[7];
	printf("请输入密码:\n");
	get_passwd(password,sizeof(password));
	printf("请再次输入密码:\n");
	get_passwd(repassword,sizeof(repassword));
	
	if(strcmp(password,repassword))			
	{
		show_msg("两次密码输入不一,修改失败!",TIME);
		return;
	}
	show_msg("修改成功!",TIME);
}



void show_operator(void)     			//遍历操作员
{

	for(int i=0; i<operator_cnt; i++)
	{
		printf("ID:%d 密码:%s\n",
			operator[i].id,
			operator[i].password);
	}
	anykey_continue();
}


Student student[STUDENT_MAX];
int student_cnt;

void admin_student_menu(void)           //管理员管理学生菜单
{
	system("clear");
    puts("---------------------------------------------------------\n");
	puts("*****学生系统*****");
	puts("1、增加学生");
	puts("2、删除学生");
	puts("3、修改学生信息");
	puts("4、查看学生信息");
	puts("5、返回上一级");
    puts("---------------------------------------------------------\n");
}

void operator_choicechange_menu(void)        //选择哪一项更改
{
	system("clear");
	puts("---------------------------------------------------------\n");
	puts("*****请选择要修改的数据*****");
	puts("1、姓名");
	puts("2、密码");
	puts("3、退出");
	puts("---------------------------------------------------------\n");
}


//管理学生

//学生登录，登录成功返回值为1，登录失败返回值为0
int student_login(void)
{
	char id[10];
	// 输入后勤人员工号
	system("clear");
    printf("请输入学号：");
	get_str(id,sizeof(id));

	// 查询后勤人员工号是否存在
	int index1 = query_studentid(id);
	if(-1 == index1)
	{
		show_msg("该学号不存在，请检查学号是否正确！",TIME);
		return -1;
	}

	// 输入密码
	char passwd[7];
	printf("请输入密码：");
	get_passwd(passwd,sizeof(passwd));

	// 比较密码
	if(0 == strcmp(student[index1].password,passwd))
	{
		//记录登录成功的后勤人员下标
        show_msg("登录成功！",TIME);              //登录成功，返回值1
		return index1;
	}
  else
    {
		show_msg("密码错误！请重新输入",TIME);
    }
  return -1;
}

void register_student(void)               
{

	for(;;)
	{
		admin_student_menu();

		switch(get_cmd('1','5'))
		{
		case '1': add_student();	break;      //增加学生
		case '2': reduce_student();	break;    	//删除学生
		case '3': change_student();	break;  	//修改学生
		case '4': show_student();	break;      //查询学生
		case '5': return;
		}
	}
	
}

void add_student(void)     //增加新学生
{
	// 判断用户是否已满
	if(student_cnt >= STUDENT_MAX)
	{
		show_msg("学生人数已满,无法添加！",TIME);
		return;
	}

    // 输入ID
	operator[operator_cnt].id = get_personid();//自动获取空余ID函数;
	// 检查ID是否被占用
    if(-1 < query_operatorid(operator[operator_cnt].id))
    {
	show_msg("该ID已被占用，无法注册！",TIME);
	return; 
    }
	
	printf("后勤人员ID为:%d\n",operator[operator_cnt].id);

	// 输入用户名
	printf("请输入名字(简写)：\n");
	get_str(student[student_cnt].name,sizeof(student[student_cnt].name));
	

	// 输入密码
	char password[7];
	char repassword[7];
	printf("请输入密码:\n");
	get_passwd(password,sizeof(password));
	printf("请再次输入密码:\n");
	get_passwd(repassword,sizeof(repassword));
	
	if(strcmp(password,repassword))			
	{
		show_msg("两次密码输入不一致,注册失败!",TIME);
		return;
	}

	strcpy(student[student_cnt].password,password);

    student_cnt++;

	show_msg("注册成功!",TIME);
}

void reduce_student(void)  //删除学生
{
	system("clear");
	printf("请输入要删除学生的ID:\n");

	char id[10];
	get_str(id,sizeof(id));
	int place = query_studentid(id);
	if(-1 == place)
	{
		show_msg("没有该ID的学生!\n",TIME);
		return;
	}
	if(yes_or_no())
		{
			// 把最后一个学生移动给要删除的学生位置
			student[place] = student[student_cnt-1];
			// 学生人数减1
			student_cnt--;
			show_msg("删除学生成功！",TIME);
		}
	else
	{
		show_msg("取消删除操作！",TIME);
	}
}

void change_student(void)              //修改学生信息
{
	system("clear");
	printf("请输入学生ID:\n");

	char id[10];
	get_str(id,sizeof(id));

	int position = query_studentid(id);
	if(-1 == position)
	{
		show_msg("没有该ID的学生!\n",TIME);
		return;
	}

	printf("请选择您要修改的信息\n"); 
    printf("1.姓名  2.密码  3.不修改\n");
	switch(get_cmd('1','3'))
	{
	case '1':student_name_change(position);break;
	case '2':student_passwd_change(position);break;
	}
}


void student_name_change(int position) //修改姓名
{
	printf("姓名:%s\n",student[position].name);//显示原来姓名
	printf("请输入更改后操作员信息:\n");
	get_str(student[position].name,sizeof(student[position].name));
}




void student_passwd_change(int place) 					//修改学生的密码
{
	char password1[7];
	char password2[7];
	printf("您的密码修改为:\n");
	get_passwd(password1,sizeof(password1));
	printf("请再次输入您的密码:\n");
	get_passwd(password2,sizeof(password2));

	if(!strcmp(password1,password2))			//两次输入密码是否一致
	{
		if(0 == strcmp(password1,student[place].password))
		{
			show_msg("输入与上次密码一致,无需修改!",TIME);
			return;
		}
		strcpy(student[place].password,password1);
		show_msg("修改成功!",TIME);
		return;
	}
	show_msg("密码输入不一致,修改失败!",TIME);
}

void show_student(void)     // 遍历学生
{

	for(int i=0; i<student_cnt; i++)
	{
		printf("ID:%s 姓名:%s 密码:%s\n",
			student[i].student_id,
			student[i].name,
			student[i].password);
	}
	anykey_continue();
}


