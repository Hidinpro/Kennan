#ifndef SCORE_H
#define SCORE_H
#include "manager.h"
#include <string.h>
#include <stdlib.h>

#define GIFT_MAX 10

//礼品结构体
 typedef struct Gift{
	int id;
	char name[20];
	int score;
}Gift;

extern Gift gifts[GIFT_MAX];

//积分兑换
void exchange1(void);
//礼品兑换界面
void exchange(void);

#endif//SCORE_H
