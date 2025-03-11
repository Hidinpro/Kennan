#include <stdio.h>
#include "game2048.h"
#include "game2048_tools.h"

uint16_t body[4][4];
uint32_t current_score;
uint32_t highest_score;
bool is_rand = true;

void init_game2048(void)
{
	// ������߷�
	highest_score = get_highest_score();
	// ���Լ��ش浵 
	load_game();
}

void start_game2048(void)
{
	for(;;)
	{
		// �������һ������
		rand_num();
		
		// ��ʾ���� 
		show_body();
		
		// �ж���Ϸ�Ƿ���� 
		if(is_gameover())
		{
			puts("��Ϸ�����Ƿ��˳�(y/n)��");
			if('y' == get_cmd())
				return; 
				
			new_game();
			continue;
		}
		
		// �ٶ����β����ƶ����� 
		is_rand = false;
		
		// ��ȡ�����
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
	printf("��ͻ�Ʊ�������߷ּ�¼����������Ĵ�����");
	scanf("%s",name);
	add_history(name);
	
	printf("1���鿴��������\n 2���鿴��������\n 3���˳�\n");
	list_history();
}
