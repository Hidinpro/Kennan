#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <tools.h>
#include <msg_queue.h>
#include "client_logon.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int cli_fd;

void error_exit(const char* func)
{
	printf("%s: %m\n", func);
	exit(EXIT_FAILURE);
}
void create(void)
{
	ctos.type = M_CREATE;

	put_msg("请输入姓名：");
	get_str(ctos.acc.name,sizeof(ctos.acc.name));

	put_msg("请输入证件号：");
	get_str(ctos.acc.card,sizeof(ctos.acc.card));

	put_msg("请输入密码：");
	get_passwd(ctos.acc.passwd,sizeof(ctos.acc.passwd),true);

	put_msg("请输入预存的金额：");
	scanf("%f",&ctos.acc.balance);

	send(cli_fd,&ctos,sizeof(ctos), 0);
	debug("进程%u向服务端发送开户请求！\n",ctos.pid);
	recv(cli_fd,&stoc,sizeof(stoc), 0);
	debug("进程%ld从服务端接收开户结果！\n",stoc.type);
	put_msg("%s\n",stoc.str);
}

void destroy(void)
{
	ctos.type = M_DESTROY;
	
	put_msg("请输入卡号：");
	get_str(ctos.acc.bank,sizeof(ctos.acc.bank));
	
	put_msg("请输入姓名：");
	get_str(ctos.acc.name,sizeof(ctos.acc.name));

	put_msg("请输入证件号：");
	get_str(ctos.acc.card,sizeof(ctos.acc.card));

	put_msg("请输入密码：");
	get_passwd(ctos.acc.passwd,sizeof(ctos.acc.passwd),true);


	send(cli_fd,&ctos,sizeof(ctos), 0);
	debug("进程%u向服务端发送开户请求！\n",ctos.pid);
	recv(cli_fd,&stoc,sizeof(stoc), 0);
	debug("进程%ld从服务端接收开户结果！\n",stoc.type);
	put_msg("%s\n",stoc.str);

}

void login(void)
{
	ctos.type = M_LOGIN;
	
	put_msg("请输入卡号：");
	get_str(ctos.acc.bank,sizeof(ctos.acc.bank));

	put_msg("请输入密码：");
	get_passwd(ctos.acc.passwd,sizeof(ctos.acc.passwd),true);


	send(cli_fd,&ctos,sizeof(ctos), 0);
	debug("进程%u向服务端发送开户请求！\n",ctos.pid);
	
	recv(cli_fd,&stoc,sizeof(stoc), 0);
	debug("进程%ld从服务端接收开户结果！\n",stoc.type);
	
	if(NULL != strstr(stoc.str,"success"))
	{
		sscanf(stoc.str,"result:success info:%s",ctos.acc.name);
		logon();
	}
	else
		put_msg("%s\n",stoc.str);
}

void unlock(void)
{
	put_msg(__func__);
}

void menu_main(void)
{
	put_msg("***欢迎使用指针银行交易系统***\n");
	put_msg("1、开户		2、销户\n");
	put_msg("3、登录		4、解锁\n");
	put_msg("5、退出系统\n");
	put_msg("---------------------\n");
	put_msg("请输入指令：");
}

int main(int argc,const char* argv[])
{
	cli_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > cli_fd)
	{
		error_exit("socket");
	}
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(2124);
	addr.sin_addr.s_addr = inet_addr("10.0.2.15");
	socklen_t addrlen = sizeof(addr);
	if(connect(cli_fd, (struct sockaddr*)&addr, addrlen))
	{
		error_exit("connect");
	}
	ctos.pid = getpid();
	stoc.type = getpid();
	
	for(;;)
	{
		menu_main();
		switch(get_cmd('1','5'))
		{
		case '1': create(); break;
		case '2': destroy(); break;
		case '3': login(); break;
		case '4': unlock(); break;
		case '5': return EXIT_SUCCESS; 
		}
	}
}
