#include "manager.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

Logistic logistics[LOGISTIC_MAX];
//记录后勤人员数量
size_t logistic_cnt;

Vip vip[VIP_MAX];
//记录vip人员数量
size_t vip_cnt;
  																//以下是后勤人员信息管理菜单的功能函数
// 后勤人员初始登录状态																  
int login_index1 = -1;

//注销后勤人员
bool logoff_logistic(void)
{
	if(yes_or_no())
	{
		// 把最后一个后勤人员移动给要删除的后勤人员位置
		logistics[login_index1] = logistics[logistic_cnt-1];
		//后勤人员数量-1
		logistic_cnt--;
		//还原后勤人员的登录状态
		login_index1 = -1;
		show_msg("注销后勤人员成功！",TIME);
		return true;
	}
	else
	{
		show_msg("取消注销操作！",TIME);
		return false;
	}
}

//修改后勤人员密码
void modify_passwd1(void)
{
	char passwd[7];
	printf("请输入旧密码：");
	get_passwd(passwd,sizeof(passwd));

	//输入的密码和旧密码比较
	if(strcmp(passwd,logistics[login_index1].passwd))
	{
		show_msg("旧密码错误，无法修改密码！",TIME);
		return;
	}

	//输入新密码
	printf("请输入新密码：");
	get_passwd(passwd,sizeof(passwd));

	//再次输入新密码
	char repasswd[7];
	printf("请输入新密码：");
	get_passwd(repasswd,sizeof(repasswd));

	//比较两次密码是否相同
	if(0 == strcmp(passwd,repasswd))
	{
		strcpy(logistics[login_index1].passwd,passwd);
		show_msg("修改密码成功！",TIME);
	}
	else
	{
		show_msg("两次输入的密码不符，修改失败！",TIME);
	}
}

//进入后勤人员信息管理模块
void logistic_menu1(void)
{
	for(;;)
	{
		system("clear");
		puts("----------------------------------------------------------------后勤人员信息管理模块-----------------------------------------------\n");
		puts("                                                                  1、  注销  ");
		puts("                                                                  2、修改密码");
		puts("                                                                  3、退出登录");

		printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
		switch(get_cmd('1','3'))
		{
		case '1':
				if(logoff_logistic())      //调用注销函数
					return;
				else
					break;
		case '2': modify_passwd1();break;     //调用修改密码函数
		case '3': login_index1 = -1; return;
		}
	}
}
														// 以下是后勤人员登录菜单的功能函数

//查询后勤人员用户名
int query_logistic(const char* name)
{
	for(int i=0; i<logistic_cnt; i++)
	{
		if(0 == strcmp(logistics[i].name,name))
			return i;
	}
	return -1;
}

//查询后勤人员工号
int query_lgid(const char* id)
{
	for(int i=0; i<logistic_cnt; i++)
	{
		if(0 == strcmp(logistics[i].id,id))
			return i;
	}
	return -1;
}

//注册后勤人员
void register_logistic(void)
{
	// 判断后勤人员是否已满
	if(logistic_cnt >= LOGISTIC_MAX)
	{
		show_msg("后勤人员数量已满，无法添加！",TIME);
		return;
	}

	// 输入后勤人员名
	printf("请输入后勤人员名：");
	get_str(logistics[logistic_cnt].name,sizeof(logistics[logistic_cnt].name));
	
	// 检查后勤人员名是否被占用
	if(-1 < query_logistic(logistics[logistic_cnt].name))
	{
		show_msg("该名称已被占用，无法注册！",TIME);
		return;
	}

	//输入后勤人员工号
	printf("请输入后勤人员工号：");
	get_str(logistics[logistic_cnt].id,sizeof(logistics[logistic_cnt].id));

	//检查后勤人员工号是否被占用
	if(-1<query_lgid(logistics[logistic_cnt].id))
	{
		show_msg("该工号已被占用，无法注册!",TIME);
		return;
	}

	// 输入密码
	printf("请输入密码：");
	get_passwd(logistics[logistic_cnt].passwd,sizeof(logistics[logistic_cnt].passwd));

	// 再一次输入密码
	char repasswd[7];
	printf("请输入再一次输入密码：");
	get_passwd(repasswd,sizeof(repasswd));

	// 确认密码
	if(strcmp(logistics[logistic_cnt].passwd,repasswd))
	{
		show_msg("两次输入的密码不符，注册失败！",TIME);
		return;
	}

	//判断注册时用户名是不是为空
	if ('\0' == logistics[logistic_cnt].name[0])
	{
		show_msg("用户名不能为空，注册失败！",TIME);
		return;
	}

	//判断注册时工号是不是为空
	if ('\0' == logistics[logistic_cnt].id[0])
	{
		show_msg("工号不能为空，注册失败！",TIME);
		return;
	}
	
	//判断注册时用户名内是否包含了空格
	for(int i = 0;i<sizeof(logistics[logistic_cnt].name);i++)
	{
		if(32 == logistics[logistic_cnt].name[i])
		{
			show_msg("用户名输入不正确，用户名中不允许包含空格！\n注册失败！",TIME);
			return ;  
		}
	}

	//判断注册时工号内是否包含了空格
	for(int i = 0;i<sizeof(logistics[logistic_cnt].id);i++)
	{
		if(32 == logistics[logistic_cnt].id[i] || 10 == logistics[logistic_cnt].id[i])
		{
			show_msg("工号输入不正确，工号中不允许包含空格！\n注册失败！",TIME);
			return ;  
		}
	}
	//判断注册时密码中是否包含了空格或回车
	for(int i = 0;i<sizeof(logistics[logistic_cnt].passwd);i++)
	{
		if(10 == logistics[logistic_cnt].passwd[i] || 32 == logistics[logistic_cnt].passwd[i])
		{
			show_msg("密码输入不正确，密码中不允许包含空格或回车！\n注册失败！",TIME);
			return;  
		}
	}

	// 注册成功，后勤人员数量+1
	logistic_cnt++;
	show_msg("恭喜你，注册成功！",TIME);
}

//后勤人员登录，登录成功返回值为1，登录失败返回值为0
int login_logistic(void)
{
	// 输入后勤人员工号
	char id[10];
	printf("请输入后勤人员工号：");
	get_str(id,sizeof(id));

	// 查询后勤人员工号是否存在
	int index1 = query_lgid(id);
	if(-1 == index1)
	{
		show_msg("该后勤人员工号不存在，登陆失败",TIME);
		return 0;
	}

	// 输入密码
	char passwd[7];
	printf("请输入密码：");
	get_passwd(passwd,sizeof(passwd));

	// 比较密码
	if(0 == strcmp(logistics[index1].passwd,passwd))
	{
		//记录登录成功的后勤人员下标
		login_index1 = index1;
		//登录成功，返回值1
		return 1;
	}
	else
	{
		show_msg("密码错误！请重新输入",TIME);
	}
	return 0;
}

//遍历后勤人员
void show_logistic(void)
{
	printf("姓名\t工号\t密码\n");
	for(int i=0; i<logistic_cnt; i++)
	{
			printf("%s\t%s\t%s\n",
			logistics[i].name,
			logistics[i].id,
			logistics[i].passwd);
		
	}
	anykey_continue();
}

//进入后勤人员模块
void logistic_menu(void)
{
	for(;;)
	{
		system("clear");
		puts("------------------------------------------------------------------后勤人员---------------------------------------------------------\n");
		puts("                                                              1、   注册   ");
		puts("                                                              2、   登录   ");
		puts("                                                              3、遍历后勤人员");
		puts("                                                              4、返回人员管理界面");

		printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
		switch(get_cmd('1','4'))
		{
		case '1': register_logistic(); break;   //调用注册函数
		case '2': if(1 == login_logistic())     //调用登录函数
				{
				// 登录成功，进入后勤人员信息管理模块
				logistic_menu1();	
				}
				break;
		case '3': show_logistic(); break;  //调用遍历函数
		case '4': 
				return;
		}
	}
}
																	//以下是VIP用户信息管理菜单的功能函数

//vip初始登录状态
int login_index = -1;
//注销vip用户
bool logoff_vip(void)
{
	if(yes_or_no())
	{
		// 把最后一个vip用户移动给要删除的vip用户位置
		vip[login_index] = vip[vip_cnt-1];
		vip[vip_cnt-1].score = 0;    //把该vip的积分也清0
		//vip用户数量-1
		vip_cnt--;
		//还原vip用户的登录状态
		login_index = -1;
		show_msg("注销vip成功！",TIME);
		return true;
	}
	else
	{
		show_msg("取消注销操作！",TIME);
		return false;
	}
}

//修改vip用户的密码
void modify_passwd(void)
{
	char passwd[7];
	printf("请输入旧密码：");
	get_passwd(passwd,sizeof(passwd));

	//比较旧密码和输入的密码
	if(strcmp(passwd,vip[login_index].passwd))
	{
		show_msg("旧密码错误，无法修改密码！",TIME);
		return;
	}
	//输入新密码
	printf("请输入新密码：");
	get_passwd(passwd,sizeof(passwd));
	//再次输入新密码
	char repasswd[7];
	printf("请输入新密码：");
	get_passwd(repasswd,sizeof(repasswd));
	//比较两次输入的新密码
	if(0 == strcmp(passwd,repasswd))
	{
		strcpy(vip[login_index].passwd,passwd);
		show_msg("修改密码成功！",TIME);
	}
	else
	{
		show_msg("两次输入的密码不符，修改失败！",TIME);
	}
}

//修改vip用户的电话
void modify_phone(void)
{
	printf("请输入新电话：");
	phone_size(vip[login_index].phone);
}

//vip信息管理模块
void vip_menu1(void)
{
	for(;;)
	{
		system("clear");
		puts("-------------------------------------------------------------VIP信息管理模块-------------------------------------------------------\n");
		puts("                                                              1、  注销  ");
		puts("                                                              2、修改密码");
		puts("                                                              3、修改电话");
		puts("                                                              4、退出登录");

		printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
		switch(get_cmd('1','4'))
		{
			case '1':
					if(logoff_vip())     //调用注销函数
						return;
			else
						break;
			case '2': modify_passwd();break;     //调用修改密码函数
			case '3': modify_phone();break;
			case '4': login_index = -1; return;
		}

	}

}
// 以下是vip用户登录菜单的功能函数

//查询vip
int query_vip(const char* name)
{
	for(int i=0; i<vip_cnt; i++)
	{
		if(0 == strcmp(vip[i].name,name))
		return i;
	}
	return -1;
}


//查询电话号码
int phone_query(char *phone)
{
	assert(NULL != phone);//确保不为空指针
 for(int i=0; i<vip_cnt; i++)
 {
    if(0 == strcmp(vip[i].phone,phone))
    return i;
 }
 return -1;
}

//vip注册
int register_vip(void)
{
	// 判断vip是否已满
	if(vip_cnt >= VIP_MAX)
	{
		show_msg("vip数量已满，无法添加！",TIME);
		return 0;
	}

	// 输入vip用户名
	printf("请输入vip用户名：");
	get_str(vip[vip_cnt].name,sizeof(vip[vip_cnt].name));
															
	// 检查vip用户名是否被占用
	if(-1 < query_vip(vip[vip_cnt].name))
	{
		show_msg("该名称已被占用，无法注册！",TIME);
		return 0;
	}

	// 输入密码
	printf("请输入密码：");
	get_passwd(vip[vip_cnt].passwd,sizeof(vip[vip_cnt].passwd));

	// 再一次输入密码
	char repasswd[7];
	printf("请输入再一次输入密码：");
	get_passwd(repasswd,sizeof(repasswd));

	// 确认密码
	if(strcmp(vip[vip_cnt].passwd,repasswd))
	{
		show_msg("两次输入的密码不符，注册失败！",TIME);
		return 0;				
	}

	//判断注册时用户名是否为空
	if ('\0' == vip[vip_cnt].name[0])
	{
		show_msg("用户名不能为空，注册失败！",TIME);
		return 0;
	}
	
	//判断注册时用户名内是否包含空格
	for(int i = 0;i<sizeof(vip[vip_cnt].name);i++)
	{
		if(32 == vip[vip_cnt].name[i])
		{
			show_msg("用户名输入不正确，用户名中不允许包含空格！\n注册失败！",TIME);
			return 0;  
		}
	}

	//判断注册时密码内是否包含空格或回车
	for(int i = 0;i<sizeof(vip[vip_cnt].passwd);i++)
	{
		if(10 == vip[vip_cnt].passwd[i] || 32 == vip[vip_cnt].passwd[i])
		{
			show_msg("密码输入不正确，密码中不允许包含空格或回车！\n注册失败！",TIME);
			return 0;  
		}
	}

	// 输入电话号
	printf("请输入用于找回密码的手机号：");
	phone_size(vip[vip_cnt].phone);    //检测输入的电话号码格式是否正确
	if(-1 < phone_query(vip[vip_cnt].phone))
	{
		show_msg("该电话已被注册，不能再次使用！",TIME);
		return 0;
	}	

	//注册成功，vip用户数量+1
	vip_cnt++;
 	int i = VIP_MAX - vip_cnt;   //记录还能注册多少vip
	printf("恭喜您，注册成功！\n还可以注册%d人\n",i);
	anykey_continue();
	return 1;
}

//vip登录，登录成功则返回值1.登录失败则返回值0
int login_vip(void)
{
	// 输入vip用户名
	char name[20];
	printf("请输入vip用户名：");
	get_str(name,sizeof(name));

	// 查询vip用户名是否存在
	int index = query_vip(name);
	if(-1 == index)
	{
		show_msg("该vip成员不存在，登陆失败",TIME);
		return 0;
	}

	// 输入密码
	char passwd[7];
	printf("请输入密码：");
	get_passwd(passwd,sizeof(passwd));

	// 比较密码
	if(0 == strcmp(vip[index].passwd,passwd))
	{
		//记录登录成功的vip人员下标
		login_index = index;
		// 登录成功，返回值1	
		return 1;
	}
	else
	{
		show_msg("密码错误！请重新输入",TIME);
	}
	return 0;
}

//遍历vip
void show_vip(void)
{
	printf("VIP姓名\t\t密码\t积分\t电话号码\n");
	for(int i=0; i<vip_cnt; i++)
	{
		printf("%s\t\t%s\t%d\t%s\n",
		vip[i].name,
		vip[i].passwd,
		vip[i].score,
		vip[i].phone);
	}
	anykey_continue();
}

//vip模块
void vip_menu(void)
{
	for(;;)
	{
		system("clear");
		puts("----------------------------------------------------------------VIP----------------------------------------------------------------\n");
		puts("                                                          1、   注册   ");
		puts("                                                          2、   登录   ");
		puts("                                                          3、 遍历VIP");
		puts("                                                          4、返回人员管理界面");

		printf("-----------------------------------------------------------------------------------------------------------------------------------\n");																
		switch(get_cmd('1','4'))
		{
			case '1': register_vip(); break;    //调用注册函数
			case '2': 
					if(1 == login_vip())       //调用登录函数
					{
						vip_menu1(); 
					}
					break;
			case '3': show_vip(); break;     //调用遍历函数
			case '4':
					return  ;
		}
	}
}

//进入人员管理模块
void manager_menu(void)
{
	for(;;)
	{
		system("clear");
		puts("----------------------------------------------------------------------人员管理-----------------------------------------------------\n");
		puts("                                                                    1、后勤人员");
		puts("                                                                    2、VIP  ");
		puts("                                                                    3、返回管理员界面");

		printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
		switch(get_cmd('1','3'))
		{
		case '1': logistic_menu(); break;     //进入后勤人员菜单
		case '2': vip_menu(); break;          //进入VIP菜单
		case '3':
				return;
		}
	}
}