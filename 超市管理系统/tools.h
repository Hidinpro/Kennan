#ifndef TOOLS_H
#define TOOLS_H
#include <stdio.h>
#include <stdbool.h>

#define TIME 1.2//显示时间

//检测输入的数量是否正常(整型数据)
void check_isnumber(int *number);
//检测输入的数量是否正常(浮点类型数据)
void check_isdouble(double *number);
//判断是否手机号
void phone_size(char *address);//判断给到的指针是否为手机号
//获取命令函数
char get_cmd(char start,char end);
//显示信息函数
void show_msg(const char* msg,float sec);
//任意键继续函数
void anykey_continue(void);
//获取字符串函数
char* get_str(char* str,size_t size);
//隐藏密码函数
char* get_passwd(char* passwd,size_t size);
//确认函数
bool yes_or_no(void);
//主函数界面
void main_menu(void);

//通用的二进制保存数据
int save_data(void* base,size_t size, const char *path);

//通用的二进制加载数据
int load_data(void* base,size_t size,const char *path);
//计算文件大小
size_t file_size(const char* path);

#endif//TOOLS_H
