#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <tools.h>
#include <tcp_socket.h>
#include "client_logon.h"

void create(void)
{
	ctos.type = M_CREATE;

	put_msg("请输入用户名：");
	get_str(ctos.acc.name,sizeof(ctos.acc.name));

	put_msg("请输入证件号：");
	get_str(ctos.acc.card,sizeof(ctos.acc.card));

	put_msg("请输入密码：");
	get_passwd(ctos.acc.passwd,sizeof(ctos.acc.passwd),true);

	put_msg("请输入预存的金额：");
	scanf("%f",&ctos.acc.balance);

	send_tcp(cts,&ctos,sizeof(ctos));
	recv_tcp(cts,&stoc,sizeof(stoc));
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


	send_tcp(cts,&ctos,sizeof(ctos));
	recv_tcp(cts,&stoc,sizeof(stoc));
	put_msg("%s\n",stoc.str);

}

void login(void)
{
	ctos.type = M_LOGIN;
	
	put_msg("请输入用户名：");
	get_str(ctos.acc.bank,sizeof(ctos.acc.bank));

	put_msg("请输入密码：");
	get_passwd(ctos.acc.passwd,sizeof(ctos.acc.passwd),true);


	send_tcp(cts,&ctos,sizeof(ctos));
	recv_tcp(cts,&stoc,sizeof(stoc));
	
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
	put_msg("***欢迎使用指针银行交易改图书管理系统***\n");
	put_msg("1、创建操作员账户		2、销户\n");
	put_msg("3、登录		4、解锁\n");
	put_msg("5、退出系统 \n");
	put_msg("---------------------\n");
	put_msg("请输入指令：");
}

int main(int argc,const char* argv[])
{
	if(3 != argc)
	{
		puts("Use:./server <ip> <port>");
		return EXIT_SUCCESS;
	}
	cts = create_tcp(argv[1], atoi(argv[2]));
	if(connect_tcp(cts))
	{
		return -1;
	}
	
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
