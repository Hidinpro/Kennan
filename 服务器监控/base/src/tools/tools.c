#include <getch.h>
#include <string.h>
#include <assert.h>
#include "tools.h"

// 自定义malloc函数，记录分配内存记录
void *_my_malloc(const char *file, const char *func, uint32_t line, uint32_t size)
{
	assert(NULL != file && NULL != func);
	void *ptr = malloc(size);
	printf("file:%s func:%s line:%u malloc %p \n", file, func, line, ptr);
	return ptr;
}

// 输入字符串
char *get_str(char *str, uint32_t size)
{
	assert(NULL != str && 0 != size);
	clear_stdin();

	// 限制输入时字符的个数
	str = fgets(str, size, stdin);

	// 判断是否出错
	assert(NULL != str);
	uint32_t len = strlen(str);

	// 判断最后一个字符是否是\n
	if ('\n' == str[len - 1])
		// 输入的字符不足size-1个,清理\n即可
		str[len - 1] = '\0';
	else
		// 输入的字符超过size-1输入缓冲区有垃圾
		clear_stdin();
	return str;
}

// 输入指令
char get_cmd(char start, char end)
{
	assert(start <= end);
	clear_stdin();

	for (;;)
	{
		char cmd = getch();
		if (start <= cmd && cmd <= end)
		{
			printf("%c\n", cmd);
			return cmd;
		}
	}
}

// 输入密码
char *get_passwd(char *pwd, uint32_t len, bool is_show)
{
	assert(NULL != pwd && 0 != len);
	clear_stdin();

	int index = 0;
	while (index < len - 1)
	{
		pwd[index] = getch();

		// 如果遇到\n表示用户不想再继续输入密码
		if ('\n' == pwd[index])
			break;

		// 如果遇到 退格键 表示用户想删除前面的内容
		if (127 == pwd[index])
		{
			// 前面有内容才允许删除
			if (index > 0)
			{
				index--;
				if (is_show)
					printf("\b \b");
			}
			continue;
		}

		if (is_show)
			printf("*");

		index++;
	}

	pwd[index] = '\0';
	printf("\n");

	return pwd;
}

// 输入手机号
char *get_phone(char *phone)
{
	assert(NULL != phone);
	clear_stdin();

	int index = 0;
	while (index < 11)
	{
		phone[index] = getch();
		if (127 == phone[index])
		{
			if (index > 0)
			{
				index--;
				printf("\b \b");
			}
			continue;
		}
		if (0 == index && '1' != phone[index])
			continue;
		if ('0' > phone[index] || phone[index] > '9')
			continue;
		printf("%c", phone[index++]);
	}

	phone[index] = '\0';
	printf("\n");
	return phone;
}

// 输入性别
char get_sex(void)
{
	clear_stdin();
	printf("请输入性别(女w/男m):");

	for (;;)
	{
		char sex = getch();
		if ('w' == sex || 'W' == sex || 'm' == sex || 'M' == sex)
		{
			printf("%s\n", sex_to_str(sex));
			return sex;
		}
	}
}

// 性别转汉字
const char *sex_to_str(char sex)
{
	return 'w' == sex || 'W' == sex ? "女" : "男";
}

// 操作确认
bool yes_or_no(void)
{
	clear_stdin();
	printf("是否确认此操作(是y/否n):");

	for (;;)
	{
		char cmd = getch();
		if ('y' == cmd || 'Y' == cmd)
		{
			printf("yes\n");
			return true;
		}
		if ('n' == cmd || 'N' == cmd)
		{
			printf("no\n");
			return false;
		}
	}
}

// 任意键继续
void anykey_continue(void)
{
	clear_stdin();
	printf("按任意键继续...\n");
	getch();
}

// 通用的二进制文件保存
int save_data(void *base, uint32_t size, uint32_t nmemb, const char *path)
{
	assert(NULL != base && NULL != path);

	FILE *wfp = fopen(path, "w");
	if (NULL == wfp)
	{
		printf("%s %m\n", path);
		return 0;
	}

	int cnt = fwrite(base, size, nmemb, wfp);
	fclose(wfp);
	return cnt;
}

// 通用的二进制文件读取
int load_data(void *base, uint32_t size, uint32_t nmemb, const char *path)
{
	assert(NULL != base && NULL != path);

	FILE *rfp = fopen(path, "r");
	if (NULL == rfp)
	{
		printf("%s %m\n", path);
		return 0;
	}

	int cnt = fread(base, size, nmemb, rfp);
	fclose(rfp);
	return cnt;
}

// 通用的文本文件保存
int save_text(void *base, uint32_t size, uint32_t nmemb, const char *path,
			  int (*out_struct)(const void *ptr, FILE *fp))
{
	assert(NULL != base && NULL != path);

	FILE *wfp = fopen(path, "w");
	if (NULL == wfp)
	{
		printf("%s %m\n", path);
		return 0;
	}

	int index = 0;
	while (index < nmemb)
	{
		if (0 >= out_struct(base + index * size, wfp))
			break;
		index++;
	}

	fclose(wfp);
	return index;
}

// 通用的文本文件读取
int load_text(void *base, uint32_t size, uint32_t nmemb, const char *path,
			  int (*in_struct)(void *ptr, FILE *fp))
{
	assert(NULL != base && NULL != path);

	FILE *rfp = fopen(path, "r");
	if (NULL == rfp)
	{
		printf("%s %m\n", path);
		return 0;
	}

	int index = 0;
	while (index < nmemb)
	{
		if (0 > in_struct(base + index * size, rfp))
			break;
		index++;
	}

	fclose(rfp);
	return index;
}

// 初始化ID
int init_id(const char *path, uint32_t init_id)
{
	assert(NULL != path);

	if (!access(path, F_OK))
		return 0;

	FILE *wfp = fopen(path, "w");
	if (NULL == wfp)
	{
		printf("%s %m\n", path);
		return -1;
	}

	fwrite(&init_id, 1, sizeof(init_id), wfp);
	fclose(wfp);
	return 1;
}

// 自动生成ID
int get_id(const char *path, uint32_t *idp)
{
	assert(NULL != path);

	FILE *rfp = fopen(path, "r+");
	if (NULL == rfp)
	{
		printf("%s %m\n", path);
		return -1;
	}

	fread(idp, 1, sizeof(*idp), rfp);
	rewind(rfp);
	*idp += 1;
	fwrite(idp, 1, sizeof(*idp), rfp);
	fclose(rfp);
	*idp -= 1;
	return 0;
}

