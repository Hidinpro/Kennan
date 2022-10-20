#include <stdio.h>
#include "game2048.h"
#include "game2048_tools.h"

uint16_t body[4][4];
uint32_t current_score;
uint32_t highest_score;
bool is_rand = true;

void init_game2048(void)
{
	// 加载最高分
	highest_score = get_highest_score();
	// 尝试加载存档 
	load_game();
}

void start_game2048(void)
{
	for(;;)
	{
		// 随机生成一个整数
		rand_num();
		
		// 显示界面 
		show_body();
		
		// 判断游戏是否结束 
		if(is_gameover())
		{
			puts("游戏结束是否退出(y/n)？");
			if('y' == get_cmd())
				return; 
				
			new_game();
			continue;
		}
		
		// 假定本次不能移动数字 
		is_rand = false;
		
		// 获取方向键
		switch(get_cmd())
		{
		case 296: up_move(); break;
		case 304: down_move(); break;
		case 301: right_move(); break;
		case 299: left_move(); break;
		case 's': save_game(); break;
		case 'n': new_game(); break;
		case 'q': return;	
		}
		
		if(highest_score < current_score)
			highest_score = current_score;
	}
}

void exit_game2048(void)
{
	if(highest_score <= get_highest_score())
		return;
	
	update_highest_score();
	
	char name[20];
	printf("您突破本机的最高分记录，请留下你的大名：");
	scanf("%s",name);
	add_history(name);
	
	printf("1、查看本机排名\n 2、查看网络排名\n 3、退出\n");
	list_history();
}
