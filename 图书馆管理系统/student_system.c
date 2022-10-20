#include "student_system.h"
#include "operator_system.h"
#include "admin_system.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "tools.h"

int EnterOrNot = 0;
char id[10];
int student_index = 0;

void enter(void)
{
    EnterOrNot = student1_login();
    if(EnterOrNot == 1)
    {
        student_menu();
    }
    return;
}


void student_menu(void)
{
    for(;;)
    {
            system("clear");
            puts("---------------------------------------------------------登录界面------------------------------------------------------------------\n");
            puts("                                                        1、查询图书信息  ");
            puts("                                                        2、查询借书记录  ");
            puts("                                                        3、查询还书记录  ");
            puts("                                                        4、查询预约记录  ");
            puts("                                                        5、查询逾期记录  ");		
            puts("                                                        6、返回主界面    ");
            printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
            switch(get_cmd('1','6'))
            {
            case '1': 
                book_vague_search();
                break;   //后勤人员登录函数，代码段在logistics.c文件中，通过登录后勤人员账号从而进入商品信息界面
            
            case '2': 
                show_borrow_msg(student_index);
                break;   //VIP登录函数,通过登录vip账号从而进入礼品兑换界面
            
            case '3':
                show_return_msg(student_index);
                    break;  //通过登录管理员从而进入管理员界面
            
            case '4':
                show_reserved_msg(student_index);
                break;

            case '5':
                show_over_deadline_msg(student_index);
                break;

            case '6':
                return;
            }
    }
}

//学生登录，登录成功返回值为1，登录失败返回值为0
int student1_login(void)
{
	// 输入后勤人员工号
	system("clear");
    printf("请输入学号：");
	get_str(id,sizeof(id));

	// 查询后勤人员工号是否存在
	student_index = query_stuid(id);
	if(-1 == student_index)
	{
		show_msg("该学号不存在，请检查学号是否正确！",0.75);
		return 0;
	}

	// 输入密码
	char passwd[7];
	printf("请输入密码：");
	get_passwd(passwd,sizeof(passwd));

	// 比较密码
	if(0 == strcmp(student[student_index].password,passwd))
	{
        show_msg("登录成功！",1);
		//登录成功，返回值1
		return 1;
	}
	else
	{
		show_msg("密码错误！请重新输入",2);
	}
	return 0;
}


int query_stuid(const char* id)
{
	for(int i=0; i<MAX_STUDENT; i++)
	{
		if(0 == strcmp(student[i].student_id,id))
			return i;
	}
	return -1;
}




/*
int _chinese_vague_search(char* str,char* target)
{
    int i,j,tag = 0;
    for(i = 0;i < strlen(str);i+=2){
        for(j = 0;j < strlen(target);j+=2){
            if(str[i] == target[j] && str[i+1] == target[j+1])
                tag++;
        }
    }
    return tag == strlen(str) / 2 ? 1 : 0;
}

int _english_vague_search(char* str,char* target)
{
    int i,j,tag = 0;
    for(i = 0;i < strlen(str);i++){
        for(j = 0;j < strlen(target);j++){
            if(str[i] == target[j])
                tag++;
        }
    }
    return tag == strlen(str)? 1 : 0;
}
*/

int arr[MAX_BOOKNUM] = {0};


void book_vague_search(void)
{
    char str[100];
    printf("请输入查找信息：");
    get_str(str,sizeof(str));
    int cnt = 0;

    for(int i = 0; i < bookcnt; i++)
    {
        if(strstr(indexs[i].bookname,str) != NULL)
        {
            arr[cnt++] = i;
            printf("猜您要找的是：\n");
            show_bookmsg(i);
        }
        
        /*
        else if(_vague_search(str,indexs[i].isbn))
            show_bookmsg(i);
        else if(_vague_search(str,indexs[i].publisher))
            show_bookmsg(i);
        else if(_vague_search(str,indexs[i].author))
            show_bookmsg(i);
        */
    }
    if(cnt == 0)
        {
            show_msg("无查询结果！",1);
            return;
        }
        else
        {
            printf("是否需要预定图书？ y/n");
            if(yes_or_no() == 1)
            {
                char STR[100];
                printf("请输入要预约的书名：");
                get_str(STR,sizeof(STR));
                int Book_index = query_index(STR);
                reserved_book(Book_index);
            }
        }
         anykey_continue();
}

void show_bookmsg(int i)
{
    printf("%s %s %s %s %s %d %d\n",indexs[i].addr->isbn, indexs[i].addr->bookname, indexs[i].addr->publisher,
    indexs[i].addr->author, indexs[i].addr->kind, indexs[i].addr->price, indexs[i].addr->reserved);
    
    return;
}


void reserved_book(int i)
{
    printf("是否需要预定图书？\n");
    
    if(indexs[i].addr->reserved == true)
    {
        printf("该书已被预订！\n");
        return;
    }

    size_t studentindex = 1;

    time_t now;
	struct tm *tm_now;
	time(&now);
	tm_now = localtime(&now);

    if(indexs[i].addr->allnum - indexs[i].addr->borrow_num - indexs[i].addr->broken_man - indexs[i].addr->broken_nature > 0)
    {
        printf("该书书架上还有%d本，请询问服务台进行借书。",indexs[i].addr->allnum - indexs[i].addr->borrow_num - indexs[i].addr->broken_man - indexs[i].addr->broken_nature);
    }

    if(indexs[i].addr->reserved == false && indexs[i].addr->allnum - indexs[i].addr->borrow_num - indexs[i].addr->broken_man - indexs[i].addr->broken_nature == 0)
    {
        printf("该书可预订，是否要预定 \n");
        
        if(yes_or_no() == 1)
        {

            indexs[i].addr->reserved = true;
            if(records[studentindex] == NULL)
            {
                records[studentindex] = create_node(student[studentindex].student_id, i, 1, tm_now);
            }
            else
            {
                RecordNode* last = records[studentindex];
                while(last->next != NULL)
                {
                    last = last->next;
                }
                last->next = create_node(student[studentindex].student_id, i, 1, tm_now);                
            }

            show_msg("预订成功！",1);
            return;
        }
        
        show_msg("查询完毕！",1);
        anykey_continue();
        return;
    }
}


int query_student_id_index(char* id)
{
	for(int i=0; i<MAX_STUDENT; i++)
	{
		if(!strcmp(id,student[i].student_id))
		{
			return i;
		}
	}	
	return -1;
}



/*
void save_reserved_msg(int id, const char *path,int index)
{


    int studentindex = query_student_id_index(id);

    time_t now;
	struct tm *tm_now;
	time(&now);
	tm_now = localtime(&now);


    if(records[studentindex] == NULL)
	{
		records[studentindex] = create_node(student[studentindex].student_id, bookindex, 2, tm_now);
	}
	else
	{
		records[studentindex]->next = create_node(student[studentindex].student_id, bookindex, 2, tm_now);
	}

    save_data
    save_data

}
*/

/*
	for(int i=0; i<student_cnt; i++)
	{
		RecordNode* node = records[i];
		while(node != NULL)
		{
			printf("学号: %s\n书名: %s\nnow datetime: %d-%d-%d %d:%d:%d\n",node->student_id,indexs[node->bookindex].bookname,node->time.tm_year+1900, node->time.tm_mon+1, node->time.tm_mday, node->time.tm_hour, node->time.tm_min,node->time.tm_sec);
			if(node->next != NULL)
			{
				node = node->next;
			}
			else
			{
				break;
			}			
		}		
*/

void show_borrow_msg(int i)
{
    RecordNode *node = records[i];
    while (node != NULL)
    {
        if(node->isrecord == 1)
        {
            printf("date:%-4d-%-2d-%-2d %-2d:%-2d:%-2d  学号:%-10s  书名:%-20s  -----执行借书操作\n", node->time.tm_year + 1900, node->time.tm_mon + 1, node->time.tm_mday, node->time.tm_hour, node->time.tm_min, node->time.tm_sec, node->student_id, indexs[node->bookindex].bookname);           
        }
        if (node->next != NULL)
        {
            node = node->next;
        }
        else
            break;         
    }
    anykey_continue();
}

void show_return_msg(int i)
{
    RecordNode *node = records[i];
    
    
    while (node != NULL)
    {
        if(node->isrecord == 4)
        {
            printf("date:%-4d-%-2d-%-2d %-2d:%-2d:%-2d  学号:%-10s  书名:%s  -----执行还书操作\n", node->time.tm_year + 1900, node->time.tm_mon + 1, node->time.tm_mday, node->time.tm_hour, node->time.tm_min, node->time.tm_sec, node->student_id, indexs[node->bookindex].bookname);
        }
        if (node->next != NULL)
        {
            node = node->next;
        }
        else
            break;             
    }
    anykey_continue();
}


void show_reserved_msg(int i)
{
    RecordNode *node = records[i];
    while (node != NULL)
    {
        if(node->isrecord == 2)
        {
            printf("date:%-4d-%-2d-%-2d %-2d:%-2d:%-2d  学号:%-10s  书名:%s  -----执行预约操作\n", node->time.tm_year + 1900, node->time.tm_mon + 1, node->time.tm_mday, node->time.tm_hour, node->time.tm_min, node->time.tm_sec, node->student_id, indexs[node->bookindex].bookname);
        }
        if (node->next != NULL)
        {
            node = node->next;
        }
        else
            break;        
    }
    anykey_continue();
}

void show_over_deadline_msg(int i)
{
    RecordNode *node = records[i];
    while (node != NULL)
    {
        if(node->isrecord == 2)
        {
            printf("date:%-4d-%-2d-%-2d %-2d:%-2d:%-2d  学号:%-10s  书名:%s  -----逾期了%-3d天\n", node->time.tm_year + 1900, node->time.tm_mon + 1, node->time.tm_mday, node->time.tm_hour, node->time.tm_min, node->time.tm_sec, node->student_id, indexs[node->bookindex].bookname, node->overtime);
        }
        if (node->next != NULL)
        {
            node = node->next;
        }
        else
            break;         
    }
    anykey_continue();
}

