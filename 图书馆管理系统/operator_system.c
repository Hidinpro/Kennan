#include "operator_system.h"
#include <string.h>
#include <stdlib.h>
#include "admin_system.h"

Book books[MAX_BOOKNUM];//图书数组
Index indexs[MAX_BOOKNUM];//索引表数组
RecordNode* records[MAX_RECORD];//记录数组，下标为该记录的链表的学生在学生数组student中的下标

size_t bookcnt;//书的数量
int student_cnt;//学生的数量

//内部函数
//插入排序(按书名首字母)
void insert_sort(Index* arr, size_t len, Index val)
{
	for(int k=1; k<len; k++)
	{
		for(int i=0; i<len; i++)
		{
			if(val.bookname[0] < arr[i].bookname[0])//对比首字母
			{
				for(int j=len+1; j>i; j--)
				{
					arr[j] = arr[j-1];
				}
				arr[i] = val;
				return;
			}
		}		
	}
}

//按名字查找下标
int query_index(char* isbn)
{
	for(int i=0; i<bookcnt; i++)
	{
		if(!strcmp(isbn,indexs[i].addr->isbn))//符合则返回下标
		{
			return i;
		}
	}	
	return -1;
}
// 创建节点
RecordNode* create_node(char* student_id, size_t bookindex, int isrecord, struct tm* tm_now)
{
	RecordNode* node = malloc(sizeof(RecordNode));
    strcpy(node->student_id, student_id);
    node->bookindex = bookindex;
    node->isrecord = isrecord;//1 代表借阅记录，2　代表预约记录, ３代表逾期记录
    node->overtime = 0;
    node->time = *tm_now;
    node->next = NULL;
	return node;
}

// 查询出值为data的前驱节点
RecordNode* query_list(RecordNode* head,char* isbn)
{
	RecordNode* prev = head;
	while(NULL != prev->next && strcmp(indexs[prev->next->bookindex].addr->isbn, isbn))
		prev = prev->next;


	if(NULL == prev->next)
		return NULL;

	return prev;
}
//创建索引表
void create_index(Book* book,Index* index)
{
    for(int i=0; i<bookcnt; i++)
    {   
        strcpy(index[i].bookname, book[i].bookname);
        index[i].addr = book+i; //保存书的地址
    }   
}

//文本格式保存数据
void save_borrow(RecordNode* head, const char* path)
{
	if(NULL == head)//判断头文件是否为空,不为空则保存头结点
	{
		return;
	}
	FILE* wfp = fopen(path,"w");
	if(NULL == wfp)
	{
		printf("存储数据的位置有错误，请检查！\n");
		return;
	}
	fprintf(wfp,"%s %d %d %d %d %d %d %d %d %d %d %d %d\n",
	head->student_id,
	head->bookindex,
	head->isrecord,
	head->overtime,
	head->time.tm_sec,
	head->time.tm_min,
	head->time.tm_hour,
	head->time.tm_mday,
	head->time.tm_mon,
	head->time.tm_year,
	head->time.tm_wday,
	head->time.tm_yday,
	head->time.tm_isdst);

	if(head->next == NULL)
	{
		fclose(wfp);
		wfp = NULL;
		return;
	}
	RecordNode* node = head->next;

	while(1)
	{	
		fprintf(wfp,"%s %d %d %d %d %d %d %d %d %d %d %d %d\n",
			node->student_id,
			node->bookindex,
			node->isrecord,
			node->overtime,
			node->time.tm_sec,
			node->time.tm_min,
			node->time.tm_hour,
			node->time.tm_mday,
			node->time.tm_mon,
			node->time.tm_year,
			node->time.tm_wday,
			node->time.tm_yday,
			node->time.tm_isdst);
			if(node->next == NULL)
			{
				break;
			}
			node = node->next;
	}
	fclose(wfp);
	wfp = NULL;
	return;
}

//文本格式读取
RecordNode* load_user(const char* path)
{
	FILE* rfp = fopen(path,"r");
	if(NULL == rfp)
	{
		printf("无数据可加载！\n");
		return NULL;
	}
	RecordNode* prev = malloc(sizeof(RecordNode));
		int ret = fscanf(rfp,"%s %d %d %d %d %d %d %d %d %d %d %d %d\n",
			prev->student_id,
			&prev->bookindex,
			&prev->isrecord,
			&prev->overtime,
			&prev->time.tm_sec,
			&prev->time.tm_min,
			&prev->time.tm_hour,
			&prev->time.tm_mday,
			&prev->time.tm_mon,
			&prev->time.tm_year,
			&prev->time.tm_wday,
			&prev->time.tm_yday,
			&prev->time.tm_isdst);
		if(ret != 13)
		{
			free(prev);
			return NULL;
		}	
	RecordNode* head = prev;
	while(1)
	{
		RecordNode* node = malloc(sizeof(RecordNode));
		ret = fscanf(rfp,"%s %d %d %d %d %d %d %d %d %d %d %d %d\n",
			node->student_id,
			&node->bookindex,
			&node->isrecord,
			&node->overtime,
			&node->time.tm_sec,
			&node->time.tm_min,
			&node->time.tm_hour,
			&node->time.tm_mday,
			&node->time.tm_mon,
			&node->time.tm_year,
			&node->time.tm_wday,
			&node->time.tm_yday,
			&node->time.tm_isdst);
		if(ret != 13)
		{
			free(node);
			prev->next = NULL;
			break;
		}
		prev->next = node;
		prev = prev->next;
	}
	fclose(rfp);	
	return head;
}
//加载链表
void load_list(void)
{	
	for(int i=0; i<student_cnt; i++)//每个学生对应一个链表
	{	
		char str[50] = {};
		sprintf(str,"%s.txt",student[i].student_id);//拼合出文件名
		records[i] = load_user(str);
	}
}
//存储链表
void save_list(void)
{
	for(int i=0; i<student_cnt; i++)
	{
		char str[50] = {};
		sprintf(str,"%s.txt",student[i].student_id);
		save_borrow(records[i], str);
	}	
}

//-----------------------------------------------------------------------
//操作员界面
void operator_menu(void)
{
	for(;;)
	{
    system("clear");
    printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
    puts("操作员管理界面");
	printf(" 1、图书管理 2、登记借阅人 3、图书归还管理 4、查看逾期记录　５、返回管理系统界面\n");
    printf("-----------------------------------------------------------------------------------------------------------------------------------\n");

		//获取命令并调用相应的函数
		switch(get_cmd('1','5'))
		{
			case '1':bookmanager_menu();break;
			case '2':record_borrow();break;
			case '3':bookreturn_menu();break;
			case '4':check_overtime();break;
            case '5':return;
		}
	}
}
//---------------------------------------------

//图书管理界面
void bookmanager_menu(void)
{
	for(;;)
	{
    system("clear");
    printf("-----------------------------------------------------------------------------------------------------------------------------------\n");
    puts("操作员管理界面");
	printf(" 1、增加图书\t2、删除图书\t3、图书报损\t４、遍历书\t５、返回操作员界面\n");
    printf("-----------------------------------------------------------------------------------------------------------------------------------\n");

		//获取命令并调用相应的函数
		switch(get_cmd('1','5'))
		{
			case '1':add_book();break;
			case '2':del_book();break;
			case '3':book_broken();break;
			case '4':show_book();break;
            case '5':return;
		}
	}
}
//增加图书
void add_book(void)
{
	printf("请输入书的ISBN版号\n");
	scan_size(books[bookcnt].isbn, 10);//ISBN版号
	printf("请输入书的书名\n");
    get_str(books[bookcnt].bookname, sizeof(books[bookcnt].bookname));//书名
	printf("请输入书的出版社\n");
    get_str(books[bookcnt].publisher, sizeof(books[bookcnt].publisher));//出版社
	printf("请输入书的作者\n");
    get_str(books[bookcnt].author, sizeof(books[bookcnt].author));//作者
	printf("请输入书的种类\n");
    get_str(books[bookcnt].kind, sizeof(books[bookcnt].kind));//种类
	printf("请输入书的进货数量\n");
	books[bookcnt].allnum = check_isnumber();//进货数量
	printf("请输入书的价格\n");
	books[bookcnt].price = check_isnumber();//进货数量	
	books[bookcnt].reserved = false;//是否被预定
	

	strcpy(indexs[bookcnt].bookname, books[bookcnt].bookname);//将该书对应的书名给索引表
	indexs[bookcnt].addr = &books[bookcnt];//保存书的地址
	//插入排序(按书名首字母)
	insert_sort(indexs, bookcnt, indexs[bookcnt]);//按首字母排序
	bookcnt++;
	show_msg("添加成功!\n",TIME);
	anykey_continue();
}
//遍历书
void show_book(void)
{
	for(int i=0; i<bookcnt; i++)
	{
		printf("书名:%s\nisbn:%s\n预定情况:%d\n进货数量:%d\n被借数量:%d\n人为损坏:%d\n自然损坏:%d\n\n", indexs[i].bookname, indexs[i].addr->isbn, indexs[i].addr->reserved, indexs[i].addr->allnum, indexs[i].addr->borrow_num, indexs[i].addr->broken_man, indexs[i].addr->broken_nature);
	}
	anykey_continue();
}

//删除图书
void del_book(void)
{
	printf("请输入要删除的书名\n");
	char isbn[11];
	get_str(isbn, sizeof(isbn));
	int i = query_index(isbn);
	if(-1 != i)
	{
		for(int j=i; j<bookcnt; j++)
		{
			indexs[j] = indexs[j+1]; 
		}
		bookcnt--;
		show_msg("删除成功!\n",TIME);	
		return;
	}	
	show_msg("删除失败\n",TIME);
}
//图书报损
void book_broken(void)
{
	printf("请输入要报损的书的书名\n");
	char isbn[11];
	get_str(isbn, sizeof(isbn));
	int i = query_index(isbn);//拿到对应的书在索引表中的下标
	if(-1 != i)
	{
		indexs[i].addr->broken_nature++;//人为损坏增加
		show_msg("报损成功！\n",TIME);
		return;
	}
	show_msg("报损失败！\n",TIME);
}

//---------------------------------------------
//图书借阅管理界面
//登记借阅人，获取时间
void record_borrow(void)
{
	//获取学号
	char id[11];
	get_str(id,sizeof(id));
	int studentindex = query_studentid(id);
	if(studentindex == -1)
	{
		show_msg("登录失败", TIME);
		return;
	}
	printf("请输入要借阅的书的书名\n");
	char isbn[11];
	get_str(isbn, sizeof(isbn));
	int bookindex = query_index(isbn);

	time_t now;
	struct tm *tm_now;
	time(&now);//得到从1970年到现在总秒数
	tm_now = localtime(&now);//将秒数转换成本地时间

	RecordNode* last = records[studentindex];//设为数组头结点

	if(records[studentindex] == NULL)
	{
		records[studentindex] = create_node(student[studentindex].student_id, bookindex, borrow, tm_now);
		last = records[studentindex];
	}
	else
	{
		while(last->next != NULL)
		{
			last = last->next;
		}
		last->next = create_node(student[studentindex].student_id, bookindex, borrow, tm_now);
	}
	//到此位置last成为最新的记录节点
	if(indexs[bookindex].addr->allnum-indexs[bookindex].addr->borrow_num-indexs[bookindex].addr->broken_man-indexs[bookindex].addr->broken_nature > 0)
	{
		indexs[bookindex].addr->borrow_num++;//借阅数量增加
		show_msg("借阅成功!",TIME);
	}
	else
	{
		printf("书已借完，是否预约? y/n\n");
		if(yes_or_no())
		{
			indexs[bookindex].addr->reserved = true;
			last->next->isrecord = order;
			show_msg("预约成功!", TIME);
			return;
		}
		return;
	}
	
//遍历记录
	for(int i=0; i<student_cnt; i++)
	{
		RecordNode* node = records[i];
		while(node != NULL)
		{
			printf("学号: %s\n书名: %s\nnow datetime: %d-%d-%d %d:%d:%d\n",node->student_id,indexs[node->bookindex].bookname,node->time.tm_year+1900, node->time.tm_mon+1, node->time.tm_mday, node->time.tm_hour, node->time.tm_min,node->time.tm_sec);
			node = node->next;		
		}		
	}
	anykey_continue();
}

//---------------------------------------------
//图书归还管理界面
void bookreturn_menu(void)
{
	//获取学号
	char id[11];
	get_str(id,sizeof(id));
	int studentindex = query_studentid(id);
	if(studentindex == -1)
	{
		show_msg("登录失败", TIME);
		return;
	}
	printf("请输入借阅的书的isbn\n");
	char isbn[11];
	get_str(isbn, sizeof(isbn));
	if(NULL == query_list(records[studentindex], isbn))
	{
		show_msg("学生未借此书！", TIME);
		return;
	}
	//逾期缴费
	pay_overtime(isbn, studentindex);
	//是否损坏
	is_broken(isbn, studentindex);

}
//逾期缴费
void pay_overtime(char* isbn, size_t studentindex)
{
	if(records[studentindex] != NULL)
	{
		RecordNode* node = records[studentindex];
		while(node != NULL)
		{
			if(!strcmp(isbn, indexs[node->bookindex].addr->isbn))
			{
				if(overtime == node->isrecord && 0 != node->overtime)
				{
					int money = node->overtime * 2;
					printf("需要缴费 %d 元 \n",money);
					printf("请输入手机号\n");
					char phone[12];
					scan_size(phone,11);
					node->isrecord = borrow;
					show_msg("缴费成功!",TIME);
					anykey_continue();
					return;
				}
			}
			node = node->next;		
		}		
	} 
	show_msg("无需逾期缴费!",TIME);
}
//判断报损函数
void is_broken(char* isbn, size_t studentindex)
{
	printf("图书是否损坏？y/n");
	if(yes_or_no())
	{
		printf("请支付 %d 元\n",indexs[query_index(isbn)].addr->price);
		printf("请输入手机号\n");
		char phone[12];
		scan_size(phone,11);
		show_msg("缴费成功!",TIME);		
		indexs[query_index(isbn)].addr->broken_man++;
		indexs[query_index(isbn)].addr->borrow_num--;
	}
	RecordNode* node = query_list(records[studentindex], isbn);
	node->next->isrecord = returnbook;

	if(!indexs[node->next->bookindex].addr->reserved)
	{
		indexs[node->next->bookindex].addr->borrow_num--;
	}
}

//---------------------------------------------

//查看逾期记录
void check_overtime(void)
{
	for(int i=0; i<student_cnt; i++)
	{
		RecordNode* node = records[i];
		while(node != NULL)
		{
			if(node->isrecord == overtime)
			{
				printf("学生学号：%s\n借阅书籍：%s\n逾期时间：%d\n",node->student_id, indexs[node->bookindex].bookname, node->overtime);
			}		
			node = node->next;			
		}		
	}

	show_msg("显示完成!",TIME);
	anykey_continue();
}