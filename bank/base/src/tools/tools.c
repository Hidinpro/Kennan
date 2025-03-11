#include <assert.h>
#define DEBUG
#include "tools.h"

// 1、输入字符串
char* get_str(char* str,size_t size)
{
	clear_stdin();
	assert(NULL != str && 0 != size);

	// 限制输入时字符的个数
	str = fgets(str,size,stdin);
	// 判断是否出错
	assert(NULL != str);

	size_t len = strlen(str);
	// 判断最后一个字符是否是\n
	if('\n' == str[len-1])
		// 输入的字符不足size-1个,清理\n即可
		str[len-1] = '\0';
	else
		// 输入的字符超过size-1输入缓冲区有垃圾
		clear_stdin();
	return str;
}

// 2、输入指令
char get_cmd(char start,char end)
{
	clear_stdin();
	assert(start <= end);
	
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



// 3、输入密码
char* get_passwd(char* pwd,size_t len,bool is_show)
{
	clear_stdin();
	assert(NULL != pwd && 0 != len);

	int index = 0;
	while(index < len-1)
	{
		pwd[index] = getch();

		// 如果遇到\n表示用户不想再继续输入密码
		if('\n' == pwd[index])
			break;

		// 如果遇到 退格键 表示用户想删除前面的内容
		if(127 == pwd[index])
		{
			// 前面有内容才允许删除
			if(index > 0)
			{
				index--;
				if(is_show)
					printf("\b \b");
			}
			continue;
		}

		if(is_show)
			printf("*");

		index++;
	}

	pwd[index] = '\0';
	printf("\n");

	return pwd;
}

// 4、输入手机号
char* get_phone(char* phone)
{
	clear_stdin();
	assert(NULL != phone);

	int index = 0;
	while(index < 11)
	{
		phone[index] = getch();
		if(127 == phone[index])
		{
			if(index > 0)
			{
				index--;
				printf("\b \b");
			}
			continue;
		}
		if(0 == index && '1' != phone[index])
			continue;
		if('0' > phone[index] || phone[index] > '9')
			continue;
		printf("%c",phone[index++]);
	}

	phone[index] = '\0';
	printf("\n");
	return phone;
}


// 5、输入性别
char get_sex(void)
{
	clear_stdin();
	printf("请输入性别(女w/男m):");

	for(;;)
	{
		char sex = getch();
		if('w' == sex || 'W' == sex || 'm' == sex || 'M' == sex)
		{
			printf("%s\n",sex_to_str(sex));
			return sex;
		}
	}
}

// 6、性别转汉字
const char* sex_to_str(char sex)
{
	return 'w' == sex || 'W' == sex ? "女" : "男";
}


// 7、操作确认
bool yes_or_no(void)
{
	clear_stdin();
	printf("是否确认此操作(是y/否n):");
	
	for(;;)
	{
		char cmd = getch();
		if('y' == cmd || 'Y' == cmd)
		{
			printf("yes\n");
			return true;
		}
		if('n' == cmd || 'N' == cmd)
		{
			printf("no\n");
			return false;
		}
	}
}


// 8、任意键继续
void anykey_continue(void)
{
	clear_stdin();
	printf("按任意键继续...\n");
	getch();
}

// 9、通用的二进制文件保存
int save_data(void* base,size_t size,size_t nmemb,const char* path)
{
	FILE* wfp = fopen(path,"w");
	if(NULL == wfp)
	{
		put_err("%s 打开失败，请检查路径！\n",path);
		return 0;
	}

	int cnt = fwrite(base,size,nmemb,wfp);
	fclose(wfp);
	return cnt;
}


// 10、通用的二进制文件读取
int load_data(void* base,size_t size,size_t nmemb,const char* path)
{
	FILE* rfp = fopen(path,"r");
	if(NULL == rfp)
	{
		put_err("%s 打开失败，请检查路径！\n",path);
		return 0;
	}

	int cnt = fread(base,size,nmemb,rfp);
	fclose(rfp);
	return cnt;
}

// 11、通用的文本文件保存
int save_text(void* base,size_t size,size_t nmemb,const char* path,int (*out_struct)(const void* ptr,FILE* fp))
{
	FILE* wfp = fopen(path,"w");
	if(NULL == wfp)
	{
		put_err("%s 打开失败，请检查路径！\n",path);
		return 0;
	}

	int index = 0;
	while(index < nmemb)
	{
		if(0 >= out_struct(base+index*size,wfp))
			break;
		index++;
	}

	fclose(wfp);
	return index;
}

// 12、通用的文本文件读取
int load_text(void* base,size_t size,size_t nmemb,const char* path,int (*in_struct)(void* ptr,FILE* fp))
{
	FILE* rfp = fopen(path,"r");
	if(NULL == rfp)
	{
		put_err("%s 打开失败，请检查路径！\n",path);
		return 0;
	}

	int index = 0;
	while(index < nmemb)
	{
		if(0 > in_struct(base+index*size,rfp))
			break;
		index++;
	}

	fclose(rfp);
	return index;
}

// 13、初始化ID
int init_id(const char* path,size_t init_id)
{
	if(!access(path,F_OK))
		return 0;
	FILE* wfp = fopen(path,"w");
	if(NULL == wfp)
	{
		put_err("%s 文件打开失败，请检查路径！",path);
		return -1;
	}

	fwrite(&init_id,1,sizeof(init_id),wfp);
	fclose(wfp);
	return 1;
}

// 14、自动生成ID
int get_id(const char* path,size_t* idp)
{
	FILE* rfp = fopen(path,"r+");
	if(NULL == rfp)
	{
		put_err("%s 文件打开失败，请检查路径！",path);
		return -1;
	}

	fread(idp,1,sizeof(*idp),rfp);
	rewind(rfp);
	*idp += 1;
	fwrite(idp,1,sizeof(*idp),rfp);
	fclose(rfp);
	*idp -= 1;
	return 0;
}
// 15、自定义malloc函数，记录分配内存记录
void* _my_malloc(const char* file,const char* func,size_t line,size_t size)
{
	void* ptr = malloc(size);
	printf("file:%s func:%s line:%u malloc %p \n",file,func,line,ptr);
	return ptr;
}
