#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include "monitored.h"
#include "monitor.h"

// 使用匿名结构实现单例模式(只允许结构体创建出一个结构变量)
struct
{
	int msgfd;
	int shmfd;
	const char* pname;
	Variable* arr;
} monitored;

// 初始化监控
void init_monitored(const char* pname)
{
	// 获取消息队列
	monitored.msgfd = msgget(MSG_KEY,0);
	if(0 > monitored.msgfd)
	{
		perror("msgget");
		return;
	}
	
	// 获取共享内存
	monitored.shmfd = shmget(SHM_KEY,0,0);
	if(0 > monitored.shmfd)
	{
		perror("shmget");
		return;
	}
	
	// 映射共享内存
	monitored.arr = shmat(monitored.shmfd,NULL,0);
	if((void *)-1 == monitored.arr)
	{
		monitored.arr = NULL;
		perror("shmat");
	}
	
	// 备份进程名
	monitored.pname = pname;
}

// 注册变量
int register_monitored(const char* varname)
{
	// 定义消息结构变量
	Msg msg = {Register};
	
	// 发送消息
	msgsnd(monitored.msgfd,&msg,sizeof(msg)-sizeof(msg.type),0);
	
	// 接收消息
	msgrcv(monitored.msgfd,&msg,sizeof(msg)-sizeof(msg.type),RegisterResult,0);
	
	// 如果返回的id是-1说明没有空位置了
	if(0 > msg.id)
	{
		return -1;
	}	
	
	// 根据进程号获取进程名
	strcpy(monitored.arr[msg.id].varname,varname);
	strcpy(monitored.arr[msg.id].pname,monitored.pname);
	
	// 初始化变量的值
	monitored.arr[msg.id].value = 0;
	
	return msg.id;
}

// 修改变量的值
void add_monitored(int id,int value)
{
	if(id < 0)
	{
		return;
	}
	monitored.arr[id].value += value;
}

// 注销变量
void cancel_monitored(int id)
{
	if(id < 0)
	{
		return;
	}
	
	// 取消该变量使用标记
	monitored.arr[id].usable = false;
	monitored.arr[id].value = -1;
	
	// 定义消息结构变量
	Msg msg = {Cancel,id};
	// 发送消息
	msgsnd(monitored.msgfd,&msg,sizeof(msg)-sizeof(msg.type),0);
}

// 退出监控
void exit_monitored(void)
{
	// 取消共享内在映射
	shmdt(monitored.arr);
}
