#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// 计算数组长度
#define ARR_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

// Linux系统清理输入缓冲区
#define clear_stdin() stdin->_IO_read_ptr = stdin->_IO_read_end

// 显示操作结果并等指定时间
#define wprintf(sec, ...)      \
	do                         \
	{                          \
		printf(__VA_ARGS__);   \
		fflush(stdout);        \
		usleep(sec * 1000000); \
	} while (0)

// 记录内存地址的自定义free
#ifdef DEBUG
#define my_free(ptr)                                 \
	do                                               \
	{                                                \
		printf("file:%s func:%s line:%u free %p \n", \
			   __FILE__, __func__, __LINE__, ptr);   \
		free(ptr);                                   \
	} while (0)
#else
#define my_free(ptr) free(ptr)
#endif // DEBUG



// 记录内在地址的自定义malloc
#ifdef DEBUG
#define my_malloc(size) _my_malloc(__FILE__, __func__, __LINE__, size)
#else
#define my_malloc(size) malloc(size)
#endif // DEBUG

// 自定义malloc函数，记录分配内存记录
void *_my_malloc(const char *file, const char *func, uint32_t line, uint32_t size);

// 输入字符串
char *get_str(char *str, uint32_t len);

// 输入指令
char get_cmd(char start, char end);

// 输入密码
char *get_passwd(char *pwd, uint32_t len, bool is_show);

// 输入手机号
char *get_phone(char *phone);

// 输入性别
char get_sex(void);

// 性别转汉字
const char *sex_to_str(char sex);

// 操作确认
bool yes_or_no(void);

// 任意键继续
void anykey_continue(void);

// 初始化ID
int init_id(const char *path, uint32_t init_id);

// 自动生成ID
int get_id(const char *path, uint32_t *idp);

// 通用的二进制文件保存
int save_data(void *base, uint32_t size, uint32_t nmemb, const char *path);

// 通用的二进制文件读取
int load_data(void *base, uint32_t size, uint32_t nmemb, const char *path);

// 通用的二进制文件保存
int save_text(void *base, uint32_t size, uint32_t nmemb, const char *path,
			  int (*out_struct)(const void *ptr, FILE *fp));

// 通用的二进制文件读取
int load_text(void *base, uint32_t size, uint32_t nmemb, const char *path,
			  int (*in_struct)(void *ptr, FILE *fp));
			  
#endif // TOOLS_H
