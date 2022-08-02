#ifndef SALE_H
#define SALE_H
#include "logistics.h"

#define CARTS_MAX 100//购物车最大数

//购物车结构体
typedef struct Shopping_Cart
{
    int id;
    char name[100];
    int num;   
    double price;

}Shopping_Cart;

//记录购物车数量
extern size_t carts_cnt;

//购物车数组
extern Shopping_Cart shopping_carts[CARTS_MAX];

//显示商品菜单
void show_goods(void);
//显示商品界面
void goods_menu(void);
//添加购物车函数
void shopping(void);
//遍历购物车
int show_carts(void);
//结账函数
void check_out(void);
//查找商品用户版
void user_find(void);
//清空购物车
void clear_car(void);
//将输入的名字和购物车数组内的名字比较函数
int check_repeat_carts(char* str);
//查看购物车函数
void found_carts(void);
#endif//SALE_H