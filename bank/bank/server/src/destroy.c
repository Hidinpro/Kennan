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

// 定义一个消息结构变量
Msgctos ctos;
Msgstoc stoc;

// 销户
void handle_msg(void)
{
	char path[PATH_MAX];
	sprintf(path,"%s%s",ACC_PATH,ctos.acc.bank);
	if(0 != access(path,F_OK))
	{
		sprintf(stoc.str,"result:failed info:%s 账号不存在，销户失败\n",
			ctos.acc.bank);
		return;
	}

	int fd = open(path,O_RDONLY);
	if(0 > fd)
	{
		sprintf(stoc.str,"result:failed info:服务器正在升级，请稍候再试!\n");
		put_err("打开帐户文件失败！\n");
		return;
	}

	Acc acc;
	size_t size = read(fd,&acc,sizeof(acc));
	close(fd);
	debug("读取账户信息 %u byte\n",size);
	
	// 检查余额
	if(acc.balance > 0)
	{
		sprintf(stoc.str,"result:failed info:该账户中还有余额%g，销户失败\n",
			acc.balance);
		return;
	}
	
	// 比较姓名
	if(strcmp(acc.name,ctos.acc.name))
	{
		sprintf(stoc.str,"result:failed info:姓名错误，销户失败\n");
		return;
	}
	
	// 比较证件号
	if(strcmp(acc.card,ctos.acc.card))
	{
		sprintf(stoc.str,"result:failed info:证件号错误，销户失败\n");
		return;
	}
	
	// 比较密码
	if(strcmp(acc.passwd,ctos.acc.passwd))
	{
		sprintf(stoc.str,"result:failed info:密码错误，销户失败\n");
		return;
	}
	
	char linkpath[PATH_MAX];
	sprintf(linkpath,"%s%s",ACC_PATH,ctos.acc.bank);
	
	unlink(path);
	unlink(linkpath);
	debug("删除账户文件和链接文件！\n");

	sprintf(stoc.str,"result:success info:账户%s，销户成功！\n",
		acc.bank);
	debug("销户成功！\n");
}

int main(int argc,const char* argv[])
{
	ctos.type = M_DESTROY;
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
