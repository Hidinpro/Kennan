#ifndef LOGISTICS_H
#define LOGISTICS_H
#include "manager.h"

#define COMMODITY_MAX 100//最大商品数量

#define SHOW_CM(index)do{\
    printf("id\t商品名\t额定上架数量\t现有数量\t报损数\t价格\n");\
    printf("%d\t%s\t%d\t\t%d\t\t%d\t%lf\n",commodity[index].id,commodity[index].name,commodity[index].commodity_need,commodity[index].commodity_now,commodity[index].broken_num,commodity[index].price);\
}while(0)//显示一次商品信息的宏

//商品结构体
typedef struct Commodity{
    int id;
    char name[100];
    int commodity_need;
    int commodity_now;
    int broken_num;
    double price;
}Commodity;

extern size_t commodity_cnt;//商品数量下标

extern Commodity commodity[100];//商品数组

//商品名查重函数
int check_repeat(char* str);
//遍历商品函数
void show_cm(void);
//显示商品信息界面函数
void next_menu(void);
//添加商品信息函数
void add_cm(void);
//修改商品信息函数
void change_cm(void);
//查找商品函数函数
void found_cm(void);
//查看需要补货的商品函数
void need_add(void);
//下架函数
void shelf_off(void);
//补货函数
void shelf_on(void);
//报损函数
void broken_num(void);

#endif //LOGISTICS_H