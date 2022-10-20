#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <tools.h>
#include <msg_queue.h>
#include "bank.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_exit(const char* func)
{
	printf("%s: %m\n", func);
	exit(EXIT_FAILURE);
}

// 存储队列描述符


// 定义一个消息结构变量
Msgctos ctos;
Msgstoc stoc;

// 开户
void handle_msg(void)
{
	char path[PATH_MAX];
	sprintf(path,"%s%s",ACC_PATH,ctos.acc.card);
	if(0 == access(path,F_OK))
	{
		sprintf(stoc.str,"result:failed info:%s 证件已经在本行开户\n",
			ctos.acc.card);
		return;
	}

	size_t bank;
	if(get_id(BANK_PATH,&bank))
	{
		sprintf(stoc.str,"result:failed info:服务器正在升级，请稍候再试!\n");
		put_err("自动生成ID失败！\n");
		return;
	}

	sprintf(ctos.acc.bank,"%u",bank);
	debug("自动生成银行卡号:%s\n",ctos.acc.bank);

	// 初始化锁定状态
	ctos.acc.lock = 0;

	int fd = open(path,O_WRONLY|O_CREAT,0600);
	if(0 > fd)
	{
		sprintf(stoc.str,"result:failed info:服务器正在升级，请稍候再试!\n");
		put_err("创建帐户文件失败！\n");
		return;
	}

	size_t size = write(fd,&ctos.acc,sizeof(ctos.acc));
	close(fd);
	debug("写入账户信息 %u byte\n",size);

	char linkpath[PATH_MAX];
	sprintf(linkpath,"%s%s",ACC_PATH,ctos.acc.bank);
	link(path,linkpath);
	debug("创建账户的链接文件：%s %s\n",path,linkpath);

	sprintf(stoc.str,"result:success info:卡号%s 余额%g\n",
		ctos.acc.bank,ctos.acc.balance);
	debug("开户成功！\n");
}

int main(int argc,const char* argv[])
{
	init_id(BANK_PATH,2022091501);
	debug("初始化自动生成ID功能！\n");
	int cli_fd = *(int*)argv[1];
	for(;;)
	{

		//接收数据
		size_t ret = recv(cli_fd, &ctos, sizeof(ctos), 0);
		if(0 >= ret)
		{
			break;
		}
		debug("开户子进程接收到客户端%u开户请求\n",ctos.pid);

		// 处理消息
		handle_msg();

		// 返回处理结果
		debug("向客户端%ld返回开启结果\n",stoc.type);
		ret = send(cli_fd, &stoc, sizeof(stoc), 0);
		if(0 >= ret)
		{
			break;
		}
	}
	printf("通信结束\n");
	return 0;
}
