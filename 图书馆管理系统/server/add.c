#include "add.h"
#include "tools.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <getch.h>

Operator operator[OPERATOR_MAX];
int operator_cnt;
//
int query_operatorid(const int id) 		//用操作员ID 查找操作员
{
	for(int i=0; i<operator_cnt; i++)
	{
		if(id == operator[i].id)
			return i;
	}
	return -1;
}
/*
                        全局函数
*/

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