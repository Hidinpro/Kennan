#include "score.h"

Gift gifts[GIFT_MAX]= {
	{1,"牙刷",100},
	{2,"牙膏",150},
	{3,"扫把",300},
	{4,"拖把",400},
	{5,"大米",1000}
};

//vip登录，登录成功则进入礼品兑换界面
void login_vip1(void)
{
	// 输入vip用户名
	char name[20];
	printf("请输入vip用户名：");
	get_str(name,sizeof(name));

	// 查询vip用户名是否存在
	int index = query_vip(name);
	if(-1 == index)
	{
		show_msg("该vip成员不存在，登陆失败",0.75);
		return;
	}

	// 输入密码
	char passwd[7];
	printf("请输入密码：");
	get_passwd(passwd,sizeof(passwd));

	// 比较密码
	if(0 == strcmp(vip[index].passwd,passwd))
	{
		//记录登录成功的后勤人员下标
		login_index = index;
		// 登录成功，进入礼品兑换界面
		exchange();	
		return;
	}
	show_msg("密码错误，登录失败！",1);
	return;
}
//进入礼品兑换界面
void exchange()
{
	int i;					//兑换界面
	system("clear");
	printf("----------------------------------------------------------礼品兑换-----------------------------------------------------------------\n\n");
	for(i=0;i<5;++i)
	{
	printf("序号:%d   名称:%s  所需积分:%d\n",gifts[i].id,gifts[i].name,gifts[i].score);		//输出保存的礼品及对应积分
	}
	printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
	printf("当前剩余积分：%d\n",vip[login_index].score);			//调用函数查看账户所拥有的积分
	printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
		printf("是否要兑换商品\n");
		puts("1、兑换");
		puts("2、退出");
	printf("-----------------------------------------------------------------------------------------------------------------------------------\n");

		switch(get_cmd('1','2'))
		{
		case '1': exchange1(); break;
		case '2':
				return;
		}
	
}
//积分兑换
void exchange1(void)
{
	int in;	//定义in为输入的礼品序号
	for(;;)
	{
		puts("请输入需要兑换的商品序号(1~5):\n>>>");
		scanf("%d",&in);

		if(1 <= in && in<=5)//规定输入的数字段对应礼品序号
		{
			if(vip[login_index].score < gifts[in-1].score)
			//查询账户积分是否比礼品积分低	
			{
				show_msg("积分不足!\n",1);
				return exchange();
			}

			else
			{
				vip[login_index].score -= gifts[in-1].score;
				//账户积分减去礼品积分
				show_msg("兑换成功!\n",1);
				return exchange();
			}
		}
		else
		{
			show_msg("请输入1到5!\n",1);
			//重新输入礼品序号
			return exchange();
		}
	}
}
