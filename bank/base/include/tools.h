#ifndef TOOLS_H
#define TOOLS_H
#include <stdio.h>
#include <getch.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

/***** 常用的宏函数 *****/
// 1、计算数组长度
#define ARR_LEN(arr) (sizeof(arr)/sizeof(arr[0]))

// 2、Linux系统清理输入缓冲区
#define clear_stdin() stdin->_IO_read_ptr = stdin->_IO_read_end

// 3、调试用的DEBUG宏蓝色
#ifndef DEBUG
# define debug(...) do{} while(0)
#else
# define debug(...) do{		\
	printf("\033[01;34m");	\
	printf(__VA_ARGS__);	\
	printf("\033[00m");		\
}while(0)
#endif//DEBUG

// 4、显示操作结果并等指定时间
#define put_sec(sec,...) do{\
	printf("\033[01;32m");	\
	printf(__VA_ARGS__);	\
	printf("\033[00m");		\
	fflush(stdout);			\
	usleep(sec*1000000);	\
}while(0)

// 5、显示提示信息
#define put_msg(...) do{\
	printf(__VA_ARGS__);	\
	fflush(stdout);			\
}while(0)

// 6、显示警告信息
#define put_war(...) do{	\
	printf("\033[01;33m");	\
	printf(__VA_ARGS__);	\
	printf("\033[00m");		\
	fflush(stdout);			\
}while(0)

// 7、显示错误信息
#define put_err(...) do{	\
	printf("\033[01;31m");	\
	printf(__VA_ARGS__);	\
	printf("\033[00m");		\
	fflush(stdout);			\
}while(0)

// 8、记录内存地址的自定义free
#ifdef DEBUG
# define my_free(ptr) do{								\
	printf("file:%s func:%s line:%u free %p \n",	\
		__FILE__,__func__,__LINE__,ptr);			\
	free(ptr);										\
}while(0)
#else
# define my_free(ptr) free(ptr)
#endif//DEBUG


// 9、记录内在地址的自定义malloc
#ifdef DEBUG
# define my_malloc(size) _my_malloc(__FILE__,__func__,__LINE__,size)
#else
# define my_malloc(size) malloc(size)
#endif//DEBUG

/***** 常用的工具函数 *****/
// 1、输入字符串
char* get_str(char* str,size_t len);
// 2、输入指令
char get_cmd(char start,char end);
// 3、输入密码
char* get_passwd(char* pwd,size_t len,bool is_show);
// 4、输入手机号
char* get_phone(char* phone);
// 5、输入性别
char get_sex(void);
// 6、性别转汉字
const char* sex_to_str(char sex); 
// 7、操作确认
bool yes_or_no(void);
// 8、任意键继续
void anykey_continue(void);
// 9、通用的二进制文件保存
int save_data(void* base,size_t size,size_t nmemb,const char* path);
// 10、通用的二进制文件读取
int load_data(void* base,size_t size,size_t nmemb,const char* path);
// 11、通用的二进制文件保存
int save_text(void* base,size_t size,size_t nmemb,const char* path,int (*out_struct)(const void* ptr,FILE* fp));
// 12、通用的二进制文件读取
int load_text(void* base,size_t size,size_t nmemb,const char* path,int (*in_struct)(void* ptr,FILE* fp));
// 13、初始化ID
int init_id(const char* path,size_t init_id);
// 14、自动生成ID
int get_id(const char* path,size_t* idp);
// 15、自定义malloc函数，记录分配内存记录
void* _my_malloc(const char* file,const char* func,size_t line,size_t size);

#endif//TOOLS_H
