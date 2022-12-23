#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <tools.h>
#include <msg_queue.h>
#include "bank.h"

// 存储队列描述符
int msgfd;

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
	msgfd = get_msg(MSG_KEY);
	debug("转账子进程%u启动，获取消息队列msgfd = %d\n",getpid(),msgfd);	
	
	ctos.type = M_TRANSF;
	debug("设置转账子进程要接收的消息类型 type = %ld\n",ctos.type);

	for(;;)
	{
		recv_msg(msgfd,&ctos,sizeof(ctos));
		stoc.type = ctos.pid;
		debug("转账子进程接收到客户端%u转账请求\n",ctos.pid);

		// 处理消息
		handle_msg();

		// 返回处理结果
		debug("向客户端%ld返回开启结果\n",stoc.type);
		send_str_msg(msgfd,&stoc);
	}
	return 0;
}
