#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <tools.h>
#include <msg_queue.h>
#include "bank.h"



// 开户
void create_acc(Msgstoc* stoc, Msgctos* ctos)
{
	char path[PATH_MAX];
	sprintf(path,"%s%s",ACC_PATH,ctos->acc.card);
	if(0 == access(path,F_OK))
	{
		sprintf(stoc->str,"result:failed info:%s 证件已经在本行开户\n",
			ctos->acc.card);
		return;
	}

	size_t bank;
	if(get_id(BANK_PATH,&bank))
	{
		sprintf(stoc->str,"result:failed info:服务器正在升级，请稍候再试!\n");
		put_err("自动生成ID失败！\n");
		return;
	}

	sprintf(ctos->acc.bank,"%u",bank);
	debug("自动生成银行卡号:%s\n",ctos->acc.bank);

	// 初始化锁定状态
	ctos->acc.lock = 0;

	int fd = open(path,O_WRONLY|O_CREAT,0600);
	if(0 > fd)
	{
		sprintf(stoc->str,"result:failed info:服务器正在升级，请稍候再试!\n");
		put_err("创建帐户文件失败！\n");
		return;
	}

	size_t size = write(fd,&ctos->acc,sizeof(ctos->acc));
	close(fd);
	debug("写入账户信息 %u byte\n",size);

	char linkpath[PATH_MAX];
	sprintf(linkpath,"%s%s",ACC_PATH,ctos->acc.bank);
	link(path,linkpath);
	debug("创建账户的链接文件：%s %s\n",path,linkpath);

	sprintf(stoc->str,"result:success info:卡号%s 余额%g\n",
		ctos->acc.bank,ctos->acc.balance);
	debug("开户成功！\n");
}

