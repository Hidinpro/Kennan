#ifndef MANAGER_H
#define MANAGER_H
#include "tools.h"

#define LOGISTIC_MAX 100//后勤人员最大数量
#define VIP_MAX 100//vip最大数量
//后勤人员结构体
typedef struct Logistic
{
	char name[20];
	char passwd[7];
	char id[10];
}Logistic;

//VIP结构体
typedef struct Vip
{
	char name[20];
	char passwd[7];
	int score;
	char phone[12];
}Vip;

//后勤人员下标
extern int login_index1;
//VIP下标
extern int login_index;
//后勤人员的数量
extern size_t logistic_cnt;
//vip数量
extern size_t vip_cnt;

//后勤人员数组
extern Logistic logistics[LOGISTIC_MAX];
//vip数组
extern Vip vip[VIP_MAX];

//修改vip用户的电话
void modify_phone(void);
//查询电话号码
int phone_query(char *phone);
//后勤人员注销函数
bool logoff_logistic(void);
//vip注销函数
bool logoff_vip(void);
//修改后勤人员密码函数
void modify_passwd1(void);
//修改vip密码函数
void modify_passwd(void);
//后勤人员信息管理模块函数
void logistic_menu1(void);
//VIP信息管理模块函数
void vip_menu1(void);
//查询后勤人员名称函数
int query_logistic(const char* name);
//查询vip用户名函数
int query_vip(const char* name);
//查询后勤人员工号函数
int query_lgid(const char* id);
//注册后勤人员函数
void register_logistic(void);
//注册vip函数
int register_vip(void);
//登录后勤人员函数
int login_logistic(void);
//登录vip函数
int login_vip(void);
//遍历后勤人员函数
void show_logistic(void);
//后勤人员菜单函数
void logistic_menu(void);
//遍历vip函数
void show_vip(void);
//vip菜单函数
void vip_menu(void);
//人员管理菜单函数
void manager_menu(void);

#endif//MANAGER_H
