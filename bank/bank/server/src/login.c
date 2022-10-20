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

// 登录
void handle_msg(void)
{
	char path[PATH_MAX];
	sprintf(path,"%s%s",ACC_PATH,ctos.acc.bank);
	if(0 != access(path,F_OK))
	{
		sprintf(stoc.str,"result:failed info:%s 账号不存在，登录失败\n",
			ctos.acc.bank);
		return;
	}

	int fd = open(path,O_RDWR);
	if(0 > fd)
	{
		sprintf(stoc.str,"result:failed info:服务器正在升级，请稍候再试!\n");
		put_err("打开帐户文件失败！\n");
		return;
	}

	Acc acc;
	size_t size = read(fd,&acc,sizeof(acc));
	debug("读取账户信息 %u byte\n",size);
	
	// 判断账户是否被锁定
	if(acc.lock >= LOCK_CNT)
	{
		sprintf(stoc.str,"result:failed info:该账号已被锁定，登录失败\n");
		return;
	}
	
	// 比较密码
	if(0 == strcmp(acc.passwd,ctos.acc.passwd))
	{
		sprintf(stoc.str,"result:success info:%s ，登录成功！\n",acc.name);
		debug("登录成功！\n");
		close(fd);
		return;
	}

	sprintf(stoc.str,"result:failed info:密码错误，登录失败！\n");
	acc.lock++;
	lseek(fd,0,SEEK_SET);
	write(fd,&acc,sizeof(acc));
	close(fd);
	debug("记录账户号错误数 %hhd byte\n",acc.lock);
	return;
}

int main(int argc,const char* argv[])
{
	ctos.type = M_LOGIN;
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
