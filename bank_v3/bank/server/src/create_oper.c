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

// 开户
void create_oper(void)
{
	char path[PATH_MAX];
	sprintf(path,"%s%s",OPER_PATH,ctos.oper.id);
	if(0 == access(path,F_OK))
	{
		sprintf(stoc.str,"result:failed info:%s 操作员已存在\n",
			ctos.oper.id);
		return;
	}

	size_t id;
	if(get_id(BANK_PATH,&id))
	{
		sprintf(stoc.str,"result:failed info:服务器正在升级，请稍候再试!\n");
		put_err("自动生成ID失败！\n");
		return;
	}

	sprintf(ctos.oper.id,"%u",id);
	debug("自动生成操作员id:%s\n",ctos.oper.id);


	int fd = open(path,O_WRONLY|O_CREAT,0600);
	if(0 > fd)
	{
		sprintf(stoc.str,"result:failed info:服务器正在升级，请稍候再试!\n");
		put_err("创建帐户文件失败！\n");
		return;
	}

	size_t size = write(fd,&ctos.oper,sizeof(ctos.oper));
	close(fd);
	debug("写入账户信息 %u byte\n",size);

	char linkpath[PATH_MAX];
	sprintf(linkpath,"%s%s",OPER_PATH,ctos.oper.id);
	link(path,linkpath);
	debug("创建账户的链接文件：%s %s\n",path,linkpath);

	sprintf(stoc.str,"result:success info:id%s 密码%s\n",
		ctos.oper.id,ctos.oper.password);
	debug("创建成功！\n");
}

int main(int argc,const char* argv[])
{
	init_id(BANK_PATH,2022091501);
	debug("初始化自动生成ID功能！\n");

	msgfd = get_msg(MSG_KEY);
	debug("开户子进程%u启动，获取消息队列msgfd = %d\n",getpid(),msgfd);	
	
	ctos.type = M_OP_CREATE;
	debug("设置开户子进程要接收的消息类型 type = %ld\n",ctos.type);

	for(;;)
	{
		recv_msg(msgfd,&ctos,sizeof(ctos));
		stoc.type = ctos.pid;
		debug("开户子进程接收到客户端%u开户请求\n",ctos.pid);

		// 处理消息
		create_oper();

		// 返回处理结果
		debug("向客户端%ld返回开启结果\n",stoc.type);
		send_str_msg(msgfd,&stoc);
	}
	return 0;
}
