#include "logistics.h"
#include <getch.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//记录商品数量
size_t commodity_cnt;//商品数量计数
Commodity commodity[COMMODITY_MAX];//商品数组

//商品名字查重函数
int check_repeat(char* str)
{
	for(int i=0; i<commodity_cnt; i++)//遍历商品数组
	{
		if(0 == strcmp(commodity[i].name,str))//重复则返回重复数组元素下标
			return i;
	}
	return -1;
}

//check_isnumber以及check_isdouble源于tools.c


//显示商品信息界面
void next_menu(void)
{
	for(;;)
	{
    system("clear");
	show_cm();//显示商品界面
    printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
    puts("商品管理");
	printf(" 1、添加商品 2、更新商品信息 3、查找商品 4、查看需要补货的商品\n");
    printf(" 5、下架     6、补货         7、报损     8、退出系统\n");
    printf("-----------------------------------------------------------------------------------------------------------------------------------\n");

		//获取命令并调用相应的函数
		switch(get_cmd('1','8'))
		{
			case '1':add_cm();break;
			case '2':change_cm();break;
			case '3':found_cm();break;
			case '4':need_add();break;
            case '5':shelf_off();break;
            case '6':shelf_on();break;
            case '7':broken_num();break;
            case '8':
					return;
		}
	}
}
//遍历商品
void show_cm(void)
{
	printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
	printf("id\t商品名\t\t\t\t额定上架数量\t现有数量\t报损数\t价格\n");
	if(0 != commodity_cnt)//没有商品则不展示
	{
		for(int i=0; i<commodity_cnt; i++)//遍历商品数组并展示
		{
			//按顺序输出商品，制表符来保证有一定的排版
			printf("%d\t%s\t\t\t\t%d\t\t%d\t\t%d\t%.2lf\n",\
			commodity[i].id,commodity[i].name,commodity[i].commodity_need,\
			commodity[i].commodity_now,commodity[i].broken_num,commodity[i].price);
				
		}
	}
	else
	{
		printf("当前无商品!\n");
	}
}
//添加商品信息
void add_cm(void)
{ 
  	if(commodity_cnt > 99)
	{
		show_msg("已到达注册上限\n",TIME);
		return;
	}
	//输入商品信息
	printf("请输入商品的名称\n");
	get_str(commodity[commodity_cnt].name,sizeof(commodity[commodity_cnt].name));
	//查重,重复则不需要输入下边的内容
	if(0 <= check_repeat(commodity[commodity_cnt].name))
	{
		show_msg("该商品已经存在,不能添加\n",TIME);
		return;
	}
    printf("请输入额定上架数量\n");
	check_isnumber(&commodity[commodity_cnt].commodity_need);//输入额定上架数量并且检测输入的是否为数字	
	printf("请输入现有数量\n");
	check_isnumber(&commodity[commodity_cnt].commodity_now);//输入现有数量并且检测输入的是否为数字	
	//检测输入的价格是否正常
	printf("请输入价格\n");
	check_isdouble(&commodity[commodity_cnt].price);//输入数据并且检测		
    //补全剩余信息
    commodity[commodity_cnt].id = commodity_cnt + 1;
    commodity[commodity_cnt].broken_num = 0;
	printf("您还能增加%d个商品",COMMODITY_MAX - commodity_cnt);
    show_msg("商品添加成功!\n",TIME);
    commodity_cnt++;//商品数量+1
}

//修改商品信息
void change_cm(void)
{
    char name[100];//临时存放一个名字用于比较
    int index_cm;//用于存储找到的商品下边
    printf("请输入要查找的商品名\n");
    get_str(name,sizeof(name));
    index_cm = check_repeat(name);
    //检测是否存在该商品
    if(index_cm < 0)
    {
        show_msg("商品不存在或者输入有误!",TIME);
        return;
    }
    //进入修改程序
    for(;;)
    {
        system("clear");
		printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
		SHOW_CM(index_cm);//显示进行修改的商品
		printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
        printf("请输入要修改的项目 1、名字 2、额定上架数量 3、现有数量 4、价格 5、报损情况 6、退出\n");
        printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
    		switch(get_cmd('1','6'))
		{
			case '1':
                    printf("请输入名字\n");
                    get_str(commodity[index_cm].name,sizeof(commodity[index_cm].name));
                    show_msg("信息修改成功!\n",TIME);
                    break;
			case '2':
                    printf("请输入额定上架数量\n");
					check_isnumber(&commodity[index_cm].commodity_need);//输入数据并且检测		
					break;
			case '3':
                    printf("请输入现有数量\n");
					check_isnumber(&commodity[index_cm].commodity_now);//输入数据并且检测		
                    break;
            case '4':
					printf("请输入价格\n");
					check_isdouble(&commodity[index_cm].price);//输入数据并且检测						
                    break;
			case '5':                    
					printf("请输入当前报损数量\n");
					check_isnumber(&commodity[index_cm].broken_num);//输入数据并且检测					
                    break;
			case '6':return;
			
		}
    }
}

//查找商品函数
void found_cm(void)
{
    char name[100];//临时存放一个名字用于比较
    int index_cm;//用于存储找到的商品下边
    printf("请输入要查找的商品名\n");
    get_str(name,sizeof(name));
    index_cm = check_repeat(name);
    //检测是否存在该商品
    if(index_cm < 0)
    {
        show_msg("商品不存在或者输入有误!",TIME);
        return;
    }
    //存在则显示
    printf("查找成功!商品信息显示如下\n");
	SHOW_CM(index_cm);
    anykey_continue();
}

//查看需要补货的商品
void need_add(void)
{
	int add_cnt=0; //记录补货数量
    printf("需要补货的商品信息：\n");
    for(int i=0; i<commodity_cnt; i++)
    {
        if(commodity[i].commodity_now < commodity[i].commodity_need-commodity[i].broken_num)
        {
			//按顺序输出需要补货的商品列表
            printf("%d %s 需要%d件 现有%d件 需要补货%d件 其中已报损%d件\n",\
			commodity[i].id, commodity[i].name,commodity[i].commodity_need, commodity[i].commodity_now, \
			commodity[i].commodity_need-commodity[i].commodity_now,commodity[i].broken_num);
			
			add_cnt++;//需要补货的商品数量+1
        }
	}
	
	if(0 == add_cnt)//0则说明没有需要补货的商品
	{
		printf("没有商品需要补货\n");
	}
    anykey_continue();
}
//--------------------------------------------------------------------------------------------------------------------------------丁文涛

//下架函数
void shelf_off(void)
{
	//判断在数据库中是否有商品可供下架，若没有商品可供下架，则退出函数
	if(commodity_cnt < 1)
	{
		printf("目前无商品可以下架！");
		return;
	}	
		
	int ID = -1;								//临时存储id变量
	printf("请输入要下架的商品的id编号：");
	check_isnumber(&ID);						//检测输入的数量是否正确，正确则输入

	if(ID <= 0 || ID > commodity_cnt)			//判断用户输入需要下架的商品编号是否在商品数据库中存在
	{
		show_msg("请输入正确的商品编号！",TIME);		//若输入的商品编号在商品数据库中不存在，则退出函数并报错
		return;
	}

	for(int i=0;i<100;i++)						
	{
		if(ID == commodity[i].id)				//若检查到符合的商品，则显示出来
		{
			printf("商品编号:%d  商品名:%s\n",commodity[i].id,commodity[i].name);  		//若查找到需要下架的商品，则将该商品信息显示出来
			if(yes_or_no())						//若用户确认下架操作，则将商品从数据库中删除并将后面商品全部向前移一位数组
			{
				for(int j=i;j<100;j++)
  				{
  					 commodity[j] = commodity[j+1];
  					 commodity[j].id = commodity[j].id-1; 				
				}
				commodity_cnt--;				//商品数量减一
				show_msg("下架成功!",TIME);
				break;				
			}
			else
			{
				show_msg("取消下架!",TIME);
				break;
			}
			
		}

		if(i == 99)								//如果没有找到下架的商品，则退出
		{
			if(commodity[i].id != ID)
			{
				printf("未查找到符合的商品需要下架，请检查输入的商品编号是否正确！\n");
				return;
			}
		}
	}
}

//补货函数
void shelf_on(void)
{
	if(commodity_cnt < 1)
	{
		show_msg("没有商品可补货！",TIME);
		return;
	}
	system("clear");

	//更新补货清单 如果不需补货 退出
	int num;									//用来存放用户输入的商品编号
	show_cm();
	printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
	printf("请输入需要更新补货信息的商品编号：");
	//将用户输入的需要补货商品编号存入num变量中
	check_isnumber(&num);
	
	if(num<=0 || num>commodity_cnt)				//检车用户输入的商品编号是否合法，若不合法则退出此功能并显示提示
	{
		show_msg("请输入正确的商品编号！",TIME);
		return;
	}

	printf("此商品为%s\n",commodity[num-1].name);//若输入的商品编号合法，则输出需要补货的商品信息

	if(commodity[num-1].commodity_now >= commodity[num-1].commodity_need)		//若商品现有数量大于额定商品数量，则提示不需补货
	{
		show_msg("该商品不需要补货，信息更新失败！",TIME);
		if(commodity[num-1].commodity_now > commodity[num-1].commodity_need)	//并且输出商品现有数量溢出几件
		{
			//若现有数量大于额定数量，则输出溢出几件，并提示返回上一级菜单进行下架操作
			printf("该商品货架上现有%d件，多于额定数量%d件\n",commodity[num-1].commodity_now,commodity[num-1].commodity_now-commodity[num-1].commodity_need);  
			show_msg("若要进行下架操作请返回上级菜单！",TIME);
		}
		return;
	}
	commodity[num-1].commodity_now = commodity[num-1].commodity_need;			//完成商品补货操作
	show_msg("补货成功！",TIME);
}

//报损函数
void broken_num(void)
{
		if(commodity_cnt < 1)				//若数据库中没有商品信息，则显示无商品的提示		
		{
			printf("没有商品报损信息可更新！");
			return;
		}
		
		int num;							//用来存放用户输入的商品编号
		int new_broken;						//用来存放新损坏的商品件数
		printf("请输入需要更新报损信息的商品编号：\n");
		check_isnumber(&num);					//检测输入的数量是否正确，正确则输入
		
		if(num<=0 || num>commodity_cnt) 	//判断用户输入的商品编号是否越界，若越界则退出功能并显示提示
		{
			show_msg("请输入正确的商品编号！",TIME);
			return;
		}

		printf("此商品为%s   已损坏%d件\n",commodity[num-1].name,commodity[num-1].broken_num);//显示商品信息
		printf("请输入该商品新损坏的件数：\n");
		check_isnumber(&new_broken);

		if(new_broken <= 0 || new_broken >= commodity[num-1].commodity_now)						//判断输入的报错数量是否合法，若不合法则退出功能并显示错误
		{
			show_msg("请输入正确的损坏件数！",TIME);
			return;
		}

		commodity[num-1].broken_num = commodity[num-1].broken_num + new_broken;				//将新报损的件数添加到报损件数中
		commodity[num-1].commodity_now = commodity[num-1].commodity_now - new_broken;		//将现有的商品件数除去损坏件数
		show_msg("商品报损信息更新完成！",TIME);	
}
//--------------------------------------------------------------------------------------------------------------------------------丁文涛

