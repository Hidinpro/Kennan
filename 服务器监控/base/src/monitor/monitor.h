#ifndef MONITOR_H
#define MONITOR_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MSG_KEY 12348166
#define SHM_KEY 34237223

#define MONITOR_IP	 "192.168.56.103"
#define MONITOR_PORT 5566
#define LOG_PATH	 "./log/"

// 存储被监控变量的结构体
typedef struct Variable
{
	bool usable;
	char pname[20];
	char varname[20];
	uint32_t value;
	FILE* logfp;
}Variable;

typedef struct Monitor
{
	int msgfd;
	int shmfd;
	Variable* arr;
	size_t varcap;
	size_t varcnt;
}Monitor;

// 监控者可能接收到、发出的消息类型
typedef enum MsgType{
	Register=1,		// 被监控者向监控者发出的注册请求
	RegisterResult,	// 监控者被监控者返回值注册结果
	Cancel,			// 被监控者向监控者发出的注销请求
	Query,			// 查看者向监控者发出的查询请求
	QueryResult, 	// 监控者向查看者返回值查询结果
	Viewers,		// 查看者向监控者发出的查看请求
	ViewersResult,	// 监控者向查看者返回值查看结果 
}MsgType;

// 给消息队列专用的
typedef struct Msg
{
	long type;
	int id;
}Msg;

void init_monitor(size_t varcap);

void wait_monitored(void);

void wait_viewers(void);

void log_monitor(void);

void exit_monitor(void);

#endif//MONITOR_H
