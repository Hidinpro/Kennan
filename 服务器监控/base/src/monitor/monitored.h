#ifndef MONITORED_H
#define MONITORED_H

// 初始化监控
void init_monitored(const char* pname);

// 注册变量
int register_monitored(const char* varname);

// 修改变量的值
void add_monitored(int id,int value);

// 注销变量
void cancel_monitored(int id);

// 退出监控
void exit_monitored(void);

#endif//MONITORED_H
