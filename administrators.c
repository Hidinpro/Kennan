#include "administrators.h"
#include <string.h>
#include <stdlib.h>

Manager manager[MANAGER_MAX];
//记录管理员数量
size_t manager_cnt;

//以下是管理人员信息管理菜单的功能函数


//记录管理员下标
int login_index4 = -1;
//注销管理员
bool logoff_manager(void)
{
	if(yes_or_no())
	{
		// 把最后一个管理人员移动给要删除的管理人员位置
		manager[login_index4] = manager[manager_cnt-1];
		//管理人员数量-1
		manager_cnt--;
		//还原管理人员的登录状态
		login_index4 = -1;
		show_msg("注销管理人员成功！",TIME);
		return true;
	}
	else
	{
		show_msg("取消注销操作！",TIME);
		return false;
	}
}
//修改管理员密码
void modify_passwd2(void)
{
	char passwd[7];
	printf("请输入旧密码：");
	get_passwd(passwd,sizeof(passwd));

	if(strcmp(passwd,manager[login_index4].passwd))
	{
		show_msg("旧密码错误，无法修改密码！",TIME);
		return;
	}
	printf("请输入新密码：");
	get_passwd(passwd,sizeof(passwd));

	char repasswd[7];
	printf("请输入新密码：");
	get_passwd(repasswd,sizeof(repasswd));

	if(0 == strcmp(passwd,repasswd))
	{
		strcpy(manager[login_index4].passwd,passwd);
		show_msg("修改密码成功！",TIME);
	}
	else
	{
		show_msg("两次输入的密码不符，修改失败！",TIME);
	}
}

//进入管理人员信息管理模块
void manager_change(void)
{
	for(;;)
	{
	system("clear");
	puts("---------------------------------------------------------------管理人员信息管理模块------------------------------------------------\n");
	puts("                                                                  1、  注销  ");
	puts("                                                                  2、修改密码");
	puts("                                                                  3、退出登录");

	printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
	switch(get_cmd('1','3'))
	{
	case '1':
		if(logoff_manager())
			return;
		else
			break;
	case '2': modify_passwd2();break;
	case '3': login_index4 = -1; return;
	}

	}

}



// 以下是管理人员登录菜单的功能函数

//查询管理员工号
int query_managerid(const char* id)
{
	for(int i=0; i<manager_cnt; i++)
	{
		if(0 == strcmp(manager[i].id,id))
			return i;
	}
	return -1;
}

//登录管理员函数，登录成功则进入管理人员信息管理模块
int login_manager(void)
{

// 输入管理人员工号
	char id[10];
	printf("请输入管理人员工号：");
	get_str(id,sizeof(id));

	// 查询后勤人员工号是否存在
	int index1 = query_managerid(id);
	if(-1 == index1)
	{
		show_msg("该管理人员工号不存在，登陆失败",TIME);
		return 0;
	}

	// 输入密码
	char passwd[7];
	printf("请输入密码：");
	get_passwd(passwd,sizeof(passwd));

	// 比较密码
	if(0 == strcmp(manager[index1].passwd,passwd))
	{
		//记录登录成功的管理人员下标
		login_index4 = index1;
		return 1;
	}
	else
	{
		show_msg("密码错误！请重新输入",TIME);
	}
	return 0;
	
}

//遍历管理员	
void show_manager(void)
{
	printf("姓名\t工号\t密码\n");
	for(int i=0; i<manager_cnt; i++)
	{
			printf("%s\t%s\t%s\n",
			manager[i].name,
			manager[i].id,
			manager[i].passwd);
		
	}
	anykey_continue();
}

//遍历账单	
void show_account(void)
{
	printf("电话号码\t付款金额\n");
	for(int i=0; i<account_cnt; i++)
	{
			printf("%s\t%d\n",
			account[i].phone,
			account[i].bill);
		
	}
	anykey_continue();
}

//进入管理员界面
void manager_menu1(void)
{
	for(;;)
	{
		system("clear");
		puts("----------------------------------------------------------管理员界面---------------------------------------------------------------\n");
		puts("                                                          1、人员管理   ");
		puts("                                                          2、管理员信息修改");
		puts("                                                          3、遍历管理员");
		puts("                                                          4、查询转账记录");
		puts("                                                          5、返回登录界面");
		printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
		switch(get_cmd('1','5'))
		{
		case '1': manager_menu(); break;  //进入人员管理界面
		case '2': if(1 == login_manager())
				{
				 // 登录成功，进入管理人员信息管理模块
				 manager_change();	
				}
				break;
		case '3': show_manager(); break;
		case '4': show_account();break;
		case '5':
				return;
		}
	}
}
