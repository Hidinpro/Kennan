#include <stdio.h>
#include <tools.h>
#include <msg_queue.h>
#include "client_logon.h"

int msgfd;
Msgctos ctos;
Msgstoc stoc;

void save(void)
{
	put_msg(__func__);
}

void take(void)
{
	put_msg(__func__);
}

void transf(void)
{
	ctos.type = M_TRANSF;

	put_msg("请输入收款账号：");
	get_str(ctos.acc.card,sizeof(ctos.acc.card));

	put_msg("请输入转账金额：");
	scanf("%f",&ctos.acc.balance);

	send_msg(msgfd,&ctos,sizeof(ctos));
	debug("进程%u向服务端发送转账请求！\n",ctos.pid);
	recv_msg(msgfd,&stoc,sizeof(stoc));
	debug("进程%ld从服务端接收转账结果！\n",stoc.type);
	put_msg("%s\n",stoc.str);
}

void query(void)
{
	put_msg(__func__);
}

void modify(void)
{
	ctos.type = M_MODIFY;
	
	put_msg("请输入卡号：");
	get_str(ctos.acc.bank,sizeof(ctos.acc.bank));
	
	put_msg("请输入姓名：");
	get_str(ctos.acc.name,sizeof(ctos.acc.name));

	put_msg("请输入证件号：");
	get_str(ctos.acc.card,sizeof(ctos.acc.card));

	put_msg("请输入新密码：");
	get_passwd(ctos.acc.passwd,sizeof(ctos.acc.passwd),true);


	send_msg(msgfd,&ctos,sizeof(ctos));
	debug("进程%u向服务端发送改密码请求！\n",ctos.pid);
	recv_msg(msgfd,&stoc,sizeof(stoc));
	debug("进程%ld从服务端接收改密码结果！\n",stoc.type);
	put_msg("%s\n",stoc.str);
}

void menu_logon(void)
{
	put_msg("---姓名：%s 卡号：%s---\n",ctos.acc.name,ctos.acc.bank);
	put_msg("1、存款		2、取款\n");
	put_msg("3、转账		4、查余\n");
	put_msg("5、修改密码	6、返回\n");
	put_msg("----------------------------\n");
	put_msg("请输入指令：");
}

void logon(void)
{
	for(;;)
	{
		menu_logon();
		switch(get_cmd('1','6'))
		{
		case '1': save(); break;
		case '2': take(); break;
		case '3': transf(); break;
		case '4': query(); break;
		case '5': modify(); break;
		case '6': return;
		}
	}
}
