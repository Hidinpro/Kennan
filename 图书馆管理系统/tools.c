#include "tools.h"
#include <getch.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>


//检测输入的商品数量是否正常(整型数据)，并进行赋值
int check_isnumber(void)
{
	int n, judge;//n用于保存数据   judge用于储存缓冲区的值用于判断
	scanf("%d", &n);
	judge = getchar();//收集缓存区数据
	while(judge != 10 || n < 0)//如果缓存区数据不为10(10为'/n')则说明输入的不是数字,小于0则不符合要求
	{
		while(getchar() != '\n');//清理缓存区
		show_msg("请检查输入的数据并重新输入!\n",TIME);
		//继续重新输入
		scanf("%d", &n);
		judge = getchar();
	}
	//出循环则说明输入正确
	return n;
}
//检测输入的商品数量是否正常(浮点类型数据)，并进行赋值
void check_isdouble(double *number)
{
	assert(NULL != number);//确保不为空指针
	double n, judge;//n用于保存数据   judge用于储存缓冲区的值用于判断
	scanf("%lf", &n);
	judge = getchar();//收集缓存区数据
	while(judge != 10 || n < 0)////如果缓存区数据不为10(10为'/n')则说明输入的不是数字,小于0则不符合要求
	{
		while(getchar() != '\n');//清理缓存区
		show_msg("请检查输入的数据并重新输入!\n",TIME);
		//继续重新输入
		scanf("%lf", &n);
		judge = getchar();
	}
	*number = n;
	show_msg("信息修改成功!\n",TIME);
}

//获取字符并处理缓冲区
char* get_str(char* str,size_t size)
{
	assert(NULL!=str && size>1);
	
	//计算fgets读取了多少个字符
	size_t len = strlen(fgets(str,size,stdin));

	//如果最后一个字符是'\n'，则把它改为'\0'
	if('\n' == str[len-1])
		str[len-1] = '\0';
	else
		//如果最后一个字符不是'\n'，则说明缓冲区有垃圾数据，需要清理输入缓冲区
		while('\n' != getchar());
	return str;
}

//判断是否为位数符合的数字
void scan_size(char *address, size_t size)
{
	assert(NULL != address);
	int flag = 0;//标记是否有不为数字的字符，0为有，1为无
	for(;;)
	{
		char scan[size+1];//临时存储输入数据
		get_str(scan,sizeof(scan));
		//判断付款电话内是否不为数字
		if(strlen(scan) != size)
		{
			show_msg("输入有误!，请重新输入",TIME);
		}
		else
		{
			for(int i = 0;i<sizeof(scan)-1;i++)
			{
				if(scan[i] < '0' || scan[i] > '9')
				{
					show_msg("输入有误!，请重新输入",TIME);
					flag = 0;
					break;
				}
				else
				{
					flag = 1;
				}
			}
			if(flag)
			{
				strcpy(address,scan);
				return;
			}
		}
		
	}
}



//获取命令函数
char get_cmd(char start,char end)
{
	assert(start <= end);

	printf("请输入指令：");
	for(;;)
	{
		char cmd = getch();
		if(start <= cmd && cmd <= end)
		{
			printf("%c\n",cmd);
			return cmd;
		}
	}
}

//输出信息并暂停系统sec秒
void show_msg(const char* msg,float sec)
{
	printf("\33[01;32m %s \33[00m\n",msg);
	usleep(1000000*sec);
}

//按任意键继续
void anykey_continue(void)
{
	puts("按任意键继续...");
	getch();
}



//隐藏密码函数
char* get_passwd(char* passwd,size_t size)
{
	int i = 0;
	while(i<size-1)
	{
		passwd[i] = getch();
		//读取到退格键
		if(127 == passwd[i])
		{
			//数组中已输入密码
			if(i>0)
			{
				//删除一位密码
				i--;
				printf("\b \b");
			}
			continue;
		}
			i++;
			printf("*");
	}
	passwd[size-1] = '\0';
	printf("\n");
	return passwd;
}

//确认操作函数
bool yes_or_no(void)
{
	printf("请确认(y/n)?\n");
	for(;;)
	{
		char cmd = getch();
		if('y' == cmd || 'Y' == cmd || 'n' == cmd || 'N' == cmd)
			printf("%c\n",cmd);
			return 'y'==cmd || 'Y'==cmd;
	}
}

//通用的二进制保存数据
int save_data(void* base,size_t size, const char *path)
{
	assert(NULL != base && 0 !=size && NULL != path);
	FILE *wfp = fopen(path,"w");
	if(NULL == wfp)
	{
		printf("文件打开失败，无法保存数据!\n");
		return 0;
	}	
	int ret = fwrite(base,1,size,wfp);
	fclose(wfp);
	wfp = NULL;
	return ret;
}

//通用的二进制加载数据
int load_data(void* base,size_t size,const char *path)
{
	assert(NULL != base && 0 !=size && NULL != path);
	FILE *rfp = fopen(path,"a+");
	if(NULL == rfp)
	{
		printf("文件打开失败，加载数据失败!\n");
		return 0;
	}
	int ret = fread(base,1,size,rfp);
	fclose(rfp);
	rfp = NULL;
	return ret;
}

//计算文件大小
size_t file_size(const char* path)
{
	assert(NULL != path);
	FILE* rfp = fopen(path,"r");
	if(NULL == rfp)
	{
		return -1;
	}
	
	fseek(rfp,0,SEEK_END);

	size_t size = ftell(rfp);
	
	fclose(rfp);

	return size;
}


//进入主界面
void main_menu(void)
{
	system("clear");
	puts("--------------------------------------------------------------超市管理系统---------------------------------------------------------\n");
	puts("                                                              1、结账  ");
	puts("                                                              2、登录入口  ");
	puts("                                                              3、结束程序");
}
