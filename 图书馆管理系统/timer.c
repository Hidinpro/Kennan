#include "time.h"
#include "operator_system.h"
#include "admin_system.h"
#include <signal.h>
#include <unistd.h>

void sigalrm(int signum)
{
    for(int i = 0; i < student_cnt; i++)
	{
		RecordNode* node = records[i];
		if(node->isrecord == 1)
		{
			time_t record_time = mktime(&node->time);
			
			time_t now;
			struct tm *tm_now;
			time(&now);
			tm_now = localtime(&now);

			time_t now_time = mktime(tm_now);

			time_t distence = now_time - record_time;
			
			int days = distence / (3600*24);

			if(days > 30)
			{
				node->isrecord = 3;
				node->overtime++;
			}
		}
	}
}

void timer(void)
{
    // 绑定信号与函数，当信号触发时，自动执行函数
    signal(SIGALRM,sigalrm);
    alarm(3600*24); 
    return;
}
