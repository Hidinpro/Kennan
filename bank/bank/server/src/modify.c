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

// 改密码
void handle_msg(void)
{
	char path[PATH_MAX];
	sprintf(path,"%s%s",ACC_PATH,ctos.acc.bank);
	if(0 != access(path,F_OK))
	{
		sprintf(stoc.str,"result:failed info:%s 账号不存在，改密码失败\n",
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
	
	// 比较姓名
	if(strcmp(acc.name,ctos.acc.name))
	{
		sprintf(stoc.str,"result:failed info:姓名错误，改密码失败\n");
		close(fd);
		return;
	}
	
	// 比较证件号
	if(strcmp(acc.card,ctos.acc.card))
	{
		sprintf(stoc.str,"result:failed info:证件号错误，改密码失败\n");
		close(fd);
		return;
	}
	
	// 更新密码
	strcpy(acc.passwd,ctos.acc.passwd);
	lseek(fd,0,SEEK_SET);
	write(fd,&acc,sizeof(acc));
	close(fd);

	sprintf(stoc.str,"result:success info:账户%s，改密码成功！\n",acc.bank);
	debug("改密码成功！\n");
}

int main(int argc,const char* argv[])
{
	ctos.type = M_MODIFY;
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
