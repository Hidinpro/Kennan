#include "administrators.h"
#include "sale.h"
#include <stdlib.h>
#include <string.h>
#include <getch.h>

//记录商品数量
size_t commodity_cnt;
//记录购物车数量
size_t carts_cnt;
//记录vip数量
size_t vip_cnt;
//记录转账条数
size_t account_cnt;

Vip vip[VIP_MAX];//vip数组,调用自manager.h
Commodity commodity[COMMODITY_MAX];//商品数组,调用自logistics.h
Shopping_Cart shopping_carts[CARTS_MAX];//购物车数组
Account account[ACCOUNT_MAX];//记账账户数组,调用自administrators.h

//显示商品界面
void goods_menu(void)
{
    for(;;)
    {
        system("clear");
        printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
		show_goods();//展示商品
		printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
    	puts("商品管理\n");
		printf(" 1、购买商品 2、查看购物车 3、查找商品 4、结账 5、清空购物车 6、修改购物车 7、退出系统\n");
    	printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
		//获取命令并调用相应的函数
		switch(get_cmd('1','7'))
		{
			case '1':shopping();break;
			case '2':
					printf("\t\t购物车\n\n");
					show_carts();
					anykey_continue();
					break;
			case '3':user_find();break;
			case '4':check_out();break;
			case '5':
					 clear_car();
					 show_msg("购物车已清空!",TIME);
					 break;
			case '6':found_carts();
					 break;
			case '7':return;
		}
    }
}   

//清空购物车函数
void clear_car(void)
{
		for(int i=0; i<CARTS_MAX; i++)
		{
			memset(&shopping_carts[i],0,sizeof(shopping_carts[i]));//清空数组
		}
		carts_cnt = 0;
}

//将输入的名字和购物车数组内的名字比较函数
int check_repeat_carts(char* str)
{
	for(int i=0; i<CARTS_MAX; i++)
	{

		if(0 == strcmp(shopping_carts[i].name,str) && shopping_carts[i].num != 0)
			return i;
	}
	return -1;
}

//显示商品菜单
void show_goods(void)
{
		int index;//存放购物车数组的下标
		puts("商品列表\n");
	    printf("id\t商品名\t\t\t\t数量\t价格\n");
		//按顺序打印商品信息
	    if(0 != commodity_cnt)
	    {
		    for(int i=0; i<commodity_cnt; i++)
		    {
				index = check_repeat_carts(commodity[i].name);
				//换行符保证一定程度的排版
			    printf("%d\t%s\t\t\t\t%d\t%.2lf\n",\
				commodity[i].id, commodity[i].name, commodity[i].commodity_now - shopping_carts[index].num, commodity[i].price);
				//显示的数量为现有商品数量-已加入购物车的商品数量
		    }
	    }
	    else//没有商品则显示无商品
	    {
		    printf("当前无商品!\n");
    	}
}

//遍历购物车
int show_carts(void)
{
	printf("id\t商品名\t购买数量\t总价\n");
	//按顺序打印购物车信息
	if(0 < carts_cnt)
	{
		for(int i=0; i<CARTS_MAX; i++)
		{
			if(shopping_carts[i].num != 0)
			{
				//换行符保证一定程度的排版
				printf("%d\t%s\t%d\t\t%.2lf\n",shopping_carts[i].id,shopping_carts[i].name,shopping_carts[i].num,shopping_carts[i].price);		
			}
		}
	}
	else
	{
		show_msg("当前无商品!\n",TIME);
		return 0;
	}
	return 1;
}

//修改购物车函数
void found_carts(void)
{
	
	if(0 == show_carts())
	{
		return;
	}
	if(carts_cnt > CARTS_MAX-1)//判断购物车是否已满
	{
		show_msg("购物车已满，请清理",TIME);
		return;
	}
	char name[100];//临时存储商品名用于比较
    int index1;//存储商品下标
	int index2;//存储购物车下标
	printf("请输入要修改的商品名\n");
    get_str(name,sizeof(name));
    index1 = check_repeat(name);//找到商品中该名称的商品下标
	index2 = check_repeat_carts(name);//找到购物车中该名称的商品下标
    //检测是否存在该商品
    if(index1 < 0)
    {
        show_msg("商品不存在或者输入有误!",TIME);
        return;
    }
    //进入修改程序
    for(;;)
    {
		int num;//临时存放输入的数字
        system("clear");
		//打印修改界面
		printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
		show_carts();
		printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
        printf("请选择 1、删除 2、修改购买数量 3、退出\n");
        printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
    		switch(get_cmd('1','3'))//获取命令
		{
			case '1':
                    if(1 == yes_or_no())
					{
						for(int j=index2;j<CARTS_MAX;j++)//后面的购物车元素下标减少一位，达到删减的作用
  						{
  					 		shopping_carts[j] = shopping_carts[j+1];
  					 		shopping_carts[j].id = shopping_carts[j].id-1; 				
						}
						// 购物车数量减1
						carts_cnt--;
						show_msg("删除成功!",TIME);
						return;
					}
                    show_msg("取消删除!\n",TIME);
                    break;
			case '2':
                    printf("请修改商品数量\n");
					check_isnumber(&num);//检测输入的数据是否合格
					if(num > commodity[index1].commodity_now)//如果添加的数量大于现有的数量或者小于0则报错
					{
						show_msg("商品数量输入有误!",TIME);
						break;
					}//输入无误则输入购物车数量
      				shopping_carts[index2].num = num;
					shopping_carts[index2].price = shopping_carts[index2].num * commodity[index2].price;
                    show_msg("信息修改成功!\n",TIME);
                    break;
			case '3':return;
		}
    }
}
//添加到购物车函数
void shopping(void)
{
	printf("请输入要购买的商品的编号：\n");
	int id;//临时记录需要修改的id
	int num;//临时记录要修改的数量
	int index = 0;//记录修改的购物车的下标
	scanf("%d",&id);
	getch();
	printf("请输入购买数量:\n");
	check_isnumber(&num);
	for(int i=0; i<=commodity_cnt; i++)
	{
		if(id == commodity[i].id)
		{
			index = check_repeat_carts(commodity[i].name);
			if(num > (commodity[i].commodity_now - shopping_carts[index].num))
			{
				show_msg("请检查输入的商品数量!",TIME);
				return;
			}
			if(id != 1 && id != carts_cnt)//如果id为1或者id与购物车数量不同，说明商品不存在，需要增加商品数量
			{
				carts_cnt++;
			}
			else if(0 == carts_cnt)//如果购物车数量为0，说明没有商品存在，需要增加商品数量
			{
				carts_cnt++;
				shopping_carts[id-1].num = 0;//如果购物车被清空，那么把之前购物车中的商品数量清空
			}
			//用id记录购物车信息，实现修改同样的商品时不会新建一个购物车
			shopping_carts[id-1].id = commodity[i].id;
			memcpy(shopping_carts[id-1].name, commodity[i].name,CARTS_MAX);	
			shopping_carts[id-1].num = shopping_carts[id-1].num + num;	
			shopping_carts[id-1].price = shopping_carts[id-1].num * commodity[i].price;
			printf("购物车余下空位:%d\n",CARTS_MAX - carts_cnt);
			show_msg("添加成功!",TIME);
			return;
		}
	}
	show_msg("请检查输入的商品编号!",TIME);
}

//查找商品用户版
void user_find(void)
{
    char name[100];//临时记录商品名
    int index;//存放找到的商品下标
    printf("请输入要查找的商品名\n");
    get_str(name,sizeof(name));
    index = check_repeat(name);
    //检测是否存在该商品
    if(index < 0)
    {
        show_msg("商品不存在或者输入有误!",TIME);
        return;
    }
    //存在则显示
    printf("查找成功!商品信息显示如下\n");
	printf("id\t商品名\t价格\n");
	printf("%d\t%s\t%lf\n",commodity[index].id,commodity[index].name,commodity[index].price);
    anykey_continue();
}

//判断是否为VIP,是则添加积分
void vip_or_no(double sum)
{
	printf("请确认是否为VIP!\n");
	
		if(1 == yes_or_no())//如果是vip则输入vip名
		{
			for(;;)//进入vip积分增加阶段
			{
				system("clear");
				printf("按任意键继续,需要退出结账请按Q\n");
				char cmd = getch();
				if('q' == cmd || 'Q' == cmd)//如果按错则可以退出
				{
					break;
				}
				if(1 == login_vip())//进入登录函数
				{
					vip[login_index].score += sum;
					show_msg("积分添加成功!",TIME);
					return;
				}
					show_msg("输入错误!",TIME);
			}

		}	
	
	show_msg("您不是VIP!",TIME);
	//不是vip则询问是否需要注册
	printf("是否需要注册vip?\n按y进行注册，按n则直接结账\n");
	
	if(1 == yes_or_no())
	{
		for(;;)
		{
			system("clear");
			printf("按任意键继续,需要返回请按Q\n");//按错可以退出
			char cmd = getch();
			if('q' == cmd || 'Q' == cmd)//如果按错则可以退出
			{
				break;
			}
			if(1 == register_vip())//进入注册函数
			{
				vip[vip_cnt-1].score += sum;
				show_msg("积分添加成功!",TIME);
				return;
			}
		}	
	}
}


//结账函数
void check_out(void)
{
	if(0 == carts_cnt)//判断是否有商品
	{
		show_msg("当前购物车内无商品!",TIME);
		return;
	}
	printf("请问是否结账?\n");
	if(1 == yes_or_no())
	{
		double sum = 0;//存储总价
		//计算总价
		for(int i=0; i<CARTS_MAX; i++)
		{
			sum += shopping_carts[i].price;
		}
		//减少商品数量
		for(int i=0; i<commodity_cnt; i++)
		{
			for(int j=0; j<CARTS_MAX; j++)
			{		
				if(0 == strcmp(commodity[i].name,shopping_carts[j].name))
				{
					//从现有数量中扣除购物车数量
					commodity[i].commodity_now = commodity[i].commodity_now - shopping_carts[j].num;
				}
			}
		}
		//判断是否为vip
		vip_or_no(sum);
		//清空购物车
		show_msg("以下是您的账单",TIME);
		show_carts();
		printf("总价为%.2lf\n",sum);//打印总价
		//让用户输入手机号
		printf("请输入付款手机号\n");
		phone_size(account[account_cnt].phone);
		account[account_cnt].bill = sum;
		account_cnt++;
		show_msg("结账成功!",TIME);
		anykey_continue();
		clear_car();
		return;
	}
	show_msg("取消结账!",TIME);
}