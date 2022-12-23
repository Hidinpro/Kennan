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
	msgfd = get_msg(MSG_KEY);
	debug("改密码子进程%u启动，获取消息队列msgfd = %d\n",getpid(),msgfd);	
	
	ctos.type = M_MODIFY;
	debug("设置改密码子进程要接收的消息类型 type = %ld\n",ctos.type);

	for(;;)
	{
		recv_msg(msgfd,&ctos,sizeof(ctos));
		stoc.type = ctos.pid;
		debug("改密码子进程接收到客户端%u开户请求\n",ctos.pid);

		// 处理消息
		handle_msg();

		// 返回处理结果
		debug("向客户端%ld返回开启结果\n",stoc.type);
		send_str_msg(msgfd,&stoc);
	}
	return 0;
}
