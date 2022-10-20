#ifndef ADMINISTRATORS_H
#define ADMINISTRATORS_H
#include "manager.h"

#define MANAGER_MAX 100//管理员最大数
#define ACCOUNT_MAX 500//账单数最大数
#define PHONE_MAX 12 //手机号最大数
//管理员结构体
typedef struct Manager
{
	char name[20];
	char passwd[7];
	char id[10];
}Manager;

//收账信息记录结构体
typedef struct Account
{
	char phone[PHONE_MAX];
	int  bill;
}Account;

//管理员下标
extern int login_index4;
//管理员数量函数
extern size_t manager_cnt;
//收账信息数量计数
extern size_t account_cnt;

//管理员数组
extern Manager manager[MANAGER_MAX];
extern Account account[ACCOUNT_MAX];

//遍历账单	
void show_account(void);
//注销管理员函数
bool logoff_manager(void);
//修改管理员密码函数
void modify_passwd2(void);
//管理人员信息管理模块
void manager_change(void);
//查询管理员工号函数
int query_managerid(const char* id);
//遍历管理员函数
void show_manager(void);
//管理员界面模块
void manager_menu1(void);
//管理员登录函数
int login_manager(void);



#endif//ADMINISTRATORS_H
