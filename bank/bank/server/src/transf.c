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

// 转账
void handle_msg(void)
{
	char src_path[PATH_MAX];
	sprintf(src_path,"%s%s",ACC_PATH,ctos.acc.bank);
	if(0 != access(src_path,F_OK))
	{
		sprintf(stoc.str,"result:failed info:%s 账号不存在，转账失败\n",
			ctos.acc.bank);
		return;
	}
	
	char dest_path[PATH_MAX];
	sprintf(dest_path,"%s%s",ACC_PATH,ctos.acc.card);
	if(0 != access(dest_path,F_OK))
	{
		sprintf(stoc.str,"result:failed info:%s 账号不存在，转账失败\n",
			ctos.acc.card);
		return;
	}

	int src_fd = open(src_path,O_RDWR);
	int dest_fd = open(dest_path,O_RDWR);
	if(0 > src_fd || 0 > dest_fd)
	{
		sprintf(stoc.str,"result:failed info:服务器正在升级，请稍候再试!\n");
		put_err("打开帐户文件失败！\n");
		return;
	}

	Acc src_acc,dest_acc;
	size_t size = read(src_fd,&src_acc,sizeof(src_acc));
	debug("读取src账户信息 %u byte\n",size);
	
	size = read(dest_fd,&dest_acc,sizeof(dest_acc));
	debug("读取dest账户信息 %u byte\n",size);
	
	// 检查余额
	if(src_acc.balance < ctos.acc.balance)
	{
		sprintf(stoc.str,"result:failed info:该账户中还有余额%g，转账失败\n",
			src_acc.balance);
		return;
	}
	
	// 转账
	src_acc.balance -= ctos.acc.balance;
	dest_acc.balance += ctos.acc.balance;
	
	lseek(src_fd,0,SEEK_SET);
	size = write(src_fd,&src_acc,sizeof(src_acc));
	debug("写入src账户信息 %u byte\n",size);
	close(src_fd);
	
	lseek(dest_fd,0,SEEK_SET);
	size = write(dest_fd,&dest_acc,sizeof(dest_acc));
	debug("写入dest账户信息 %u byte\n",size);
	close(dest_fd);

	sprintf(stoc.str,"result:success info:%g，转账成功！\n",src_acc.balance);
	debug("转账成功！\n");
}

int main(int argc,const char* argv[])
{

	
	ctos.type = M_TRANSF;
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
