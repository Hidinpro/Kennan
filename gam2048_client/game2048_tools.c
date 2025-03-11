#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "game2048.h"
#include "game2048_tools.h"

// 随机生成一个整数 
void rand_num(void)
{
	for(;is_rand;)
	{
		int row = rand() % 4;
		int col = rand() % 4;
		if(0 == body[row][col])
		{
			switch(rand()%100)
			{
			case  0 ... 39: body[row][col] =  2; return;
			case 40 ... 69: body[row][col] =  4; return;
			case 70 ... 89: body[row][col] =  8; return;
			case 90 ... 99: body[row][col] = 16; return;
			}
		}
	}
}

// 显示界面 
void show_body(void)
{
	system("cls");
	printf("当前得分：%d 本机最高分：%d\n",current_score,highest_score);
	
	for(int r=0; r<4; r++)
	{
		puts("---------------------");
		for(int c=0; c<4; c++)
		{
			if(body[r][c]<2)
				printf("|    ");
			else if(body[r][c]<16)
				printf("| %d  ",body[r][c]);
			else if(body[r][c]<128)
				printf("| %d ",body[r][c]);
			else if(body[r][c]<1024)
				printf("| %d",body[r][c]);
			else if(body[r][c]<16384)
				printf("|%d",body[r][c]);
		}
		printf("|\n");
	}
	puts("---------------------");
}

// 判断游戏是否结束 
bool is_gameover(void)
{
	for(int r=0; r<4; r++)
	{
		for(int c=0; c<4; c++)
		{
			if(0 == body[r][c])
				return false;
		}
	}
	
	for(int r=0; r<4; r++)
	{
		for(int c=0; c<3; c++)
		{
			if(body[r][c] == body[r][c+1])
				return false;
		}
	}
	
	for(int c=0; c<4; c++)
	{
		for(int r=0; r<3; r++)
		{
			if(body[r][c] == body[r+1][c])
				return false;
		}
	}
	
	return true;
}

// 获取方向键
int get_cmd(void)
{
	int cmd = getch();
	if(224 == cmd)
		cmd += getch(); 
	return cmd;
}

// 再一来局
void new_game(void)
{
	current_score = 0;
	
	for(int r=0; r<4; r++)
	{
		for(int c=0; c<4; c++)
		{
			body[r][c] = 0;
		}
	}
	
	is_rand = true;
}



void up_move(void)
{
	for(int c=0; c<4; c++)
	{
		int merge_end = -1;
		for(int r=1; r<4; r++)
		{
			if(0 == body[r][c])
				continue;

			int end = -1;
			for(int m=r-1; m>merge_end; m--)
			{
				if(0==body[m][c] || body[r][c] == body[m][c])
					end = m;
				else
					break;
			}

			if(-1 != end)
			{
				if(body[end][c])
				{
					current_score += body[end][c]*2;
					merge_end = end;
				}
				body[end][c] += body[r][c];
				body[r][c] = 0;
				is_rand = true;
			}
		}
	}
}

void down_move(void)
{
	for(int c=0; c<4; c++)
	{
		int merge_end = 4;
		for(int r=2; r>=0; r--)
		{
			if(0 == body[r][c])
				continue;

			int end = -1;
			for(int m=r+1; m<merge_end; m++)
			{
				if(0==body[m][c] || body[r][c] == body[m][c])
					end = m;
				else
					break;
			}

			if(-1 != end)
			{
				if(body[end][c])
				{
					current_score += body[end][c]*2;
					merge_end = end;
				}
				body[end][c] += body[r][c];
				body[r][c] = 0;
				is_rand = true;
			}
		}
	}
}

void right_move(void)
{
	for(int r=0; r<4; r++)
	{
		int merge_end = 4;
		for(int c=2; c>=0; c--)
		{
			if(0 == body[r][c])
				continue;

			int end = -1;
			for(int m=c+1; m<merge_end; m++)
			{
				if(0==body[r][m] || body[r][c] == body[r][m])
					end = m;
				else
					break;
			}

			if(-1 != end)
			{
				if(body[r][end])
				{
					current_score += body[r][end]*2;
					merge_end = end;
				}
				body[r][end] += body[r][c];
				body[r][c] = 0;
				is_rand = true;
			}
		} 
	}
}

void left_move(void)
{
	for(int r=0; r<4; r++)
	{
		int merge_end = -1;
		for(int c=1; c<4; c++)
		{
			if(0 == body[r][c])
				continue;

			int end = -1;
			for(int m=c-1; m>merge_end; m--)
			{
				if(0==body[r][m] || body[r][c] == body[r][m])
					end = m;
				else
					break;
			}

			if(-1 != end)
			{
				if(body[r][end])
				{
					current_score += body[r][end]*2;
					merge_end = end;
				}
				body[r][end] += body[r][c];
				body[r][c] = 0;
				is_rand = true;
			}
		}
	}
}

// 存档
void save_game(void)
{
	FILE* wfp = fopen("game2048.dat","wb");
	if(NULL == wfp)
	{
		puts("你硬盘坏了，无法存档，赶紧去修改吧！");
		return;
	}
	
	fwrite(body,1,sizeof(body),wfp);
	fwrite(&current_score,1,sizeof(current_score),wfp);
	fclose(wfp);
	
	puts("存档成功！");
	exit(0);
}

// 加载存档
void load_game(void)
{
	FILE* rfp = fopen("game2048.dat","rb");
	if(NULL == rfp)
		return;

	fread(body,1,sizeof(body),rfp);
	fread(&current_score,1,sizeof(current_score),rfp);
	fclose(rfp);
	
	// 加载存档就再生成新的数字 
	is_rand = false;
	
	// 删除存档
	remove("game2048.dat");
}

uint32_t get_highest_score(void)
{
	FILE* rfp = fopen("highest_score.dat","rb");
	if(NULL == rfp)
		return 0;
		
	uint32_t score;
	fread(&score,1,sizeof(score),rfp);
	fclose(rfp);
	
	return score;
}

void update_highest_score(void)
{
	FILE* wfp = fopen("highest_score.dat","wb");
	if(NULL == wfp)
	{
		puts("你硬盘坏了，无法存档，赶紧去修改吧！");
		return;
	}
	
	fwrite(&highest_score,1,sizeof(highest_score),wfp);
	fclose(wfp);
}

// 添加历史记录
void add_history(const char* name)
{
	FILE* wfp = fopen("history.dat","ab");
	if(NULL == wfp)
	{
		puts("你硬盘坏了，无法存档，赶紧去修改吧！");
		return;
	}
	
	fwrite(&highest_score,1,sizeof(highest_score),wfp);
	fwrite(name,1,20,wfp);
	fclose(wfp);
}

void list_history(void)
{
	FILE* rfp = fopen("history.dat","rb");
	
	uint32_t score;
	char name[20];
	
	while(4 == fread(&score,1,sizeof(score),rfp))
	{
		fread(name,1,sizeof(name),rfp);
		printf("%u %s\n",score,name);
	}
	
	fclose(rfp);
}
