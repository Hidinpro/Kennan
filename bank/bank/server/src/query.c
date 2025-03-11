#include <stdio.h>
#include <unistd.h>
#include <tools.h>
#include <msg_queue.h>
#include "bank.h"
void error_exit(const char* func)
{
	printf("%s: %m\n", func);
	exit(EXIT_FAILURE);
}
// 存储队列描述符
int msgfd;

// 定义一个消息结构变量
Msgctos ctos = {};
Msgstoc stoc = {};

// 开户
void handle_msg(void)
{

}

int main(int argc,const char* argv[])
{
	// 获取消息队列
	msgfd = get_msg(MSG_KEY);

	for(;;)
	{
		// 从消息队列接收消息
		recv_msg(msgfd,&ctos,sizeof(ctos));

		// 处理消息
		handle_msg();

		// 返回处理结果
		send_msg(msgfd,&stoc,sizeof(stoc));
	}
	return 0;
}
