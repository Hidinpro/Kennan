#ifndef OPERATOR_SYSTEM_H
#define OPERATOR_SYSTEM_H

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "tools.h"

#define MAX_BOOKNUM 1000000
#define MAX_RECORD 10000

typedef struct Book
{
    char isbn[11];//ISBN版号
    char bookname[100];//书名
    char publisher[100];//出版社
    char author[20];//作者
    char kind[20];//种类
    size_t price;
    bool reserved;//是否被预定
    int allnum;//进货数量
    size_t borrow_num;//被借数量
    size_t broken_man;//人为损坏
    size_t broken_nature;//自然损坏
}Book;

//索引表
typedef struct Index
{
    char bookname[100]; 
    Book* addr;
}Index;

//记录结构体
typedef struct RecordNode
{
    char student_id[10];
    size_t bookindex;
    int isrecord;//1 代表借阅记录，2　代表预约记录, ３代表逾期记录 4代表还书记录
    size_t overtime;
    struct tm time;
    struct RecordNode* next;
    
}RecordNode;

enum mark //1 代表借阅记录，2　代表预约记录, ３代表逾期记录 4代表还书记录
{
    borrow = 1,
    order,
    overtime,
    returnbook
};

extern Book books[MAX_BOOKNUM];//图书数组
extern Index indexs[MAX_BOOKNUM];//索引表数组
extern RecordNode* records[MAX_RECORD];//借阅记录数组(数组下标对应学生编号)

extern size_t bookcnt;

//内部函数
//插入排序(按书名首字母)
void insert_sort(Index* arr, size_t len, Index add);
// 创建节点
RecordNode* create_node(char* student_id, size_t bookindex, int isrecord, struct tm* tm_now);
//按名字查找下标
int query_index(char* name);
// 查询出值为data的前驱节点
RecordNode* query_list(RecordNode* head,char* name);
//文本格式保存数据
void save_borrow(RecordNode* head, const char* path);
//文本格式读取
RecordNode* load_user(const char* path);
//加载链表
void load_list(void);
//存储链表
void save_list(void);
//创建索引表
void create_index(Book* book,Index* index);

//操作员界面
void operator_menu(void);
//---------------------------------------------

//图书管理界面
void bookmanager_menu(void);
//增加图书
void add_book(void);
//删除图书
void del_book(void);
//图书报损
void book_broken(void);
//遍历书
void show_book(void);

//---------------------------------------------
//登记借阅人，获取时间
void record_borrow(void);

//---------------------------------------------
//图书归还管理界面
void bookreturn_menu(void);
//逾期缴费
void pay_overtime(char* name, size_t studentindex);
//判断报损函数
void is_broken(char* name, size_t studentindex);

//---------------------------------------------
//查看逾期记录
void check_overtime(void);

#endif//OPERATOR_SYSTEM_H