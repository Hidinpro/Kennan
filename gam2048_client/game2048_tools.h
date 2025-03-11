#ifndef GAME2048_TOOLS_H
#define GAME2048_TOOLS_H

// �������һ������ 
void rand_num(void);

// ��ʾ���� 
void show_body(void);

// �ж���Ϸ�Ƿ���� 
bool is_gameover(void);

// ��ȡ�����
int get_cmd(void);

// ��һ����
void new_game(void);

// �浵
void save_game(void); 

void up_move(void);

void down_move(void);

void right_move(void);

void left_move(void);

// �浵
void save_game(void); 

// ���ش浵 
void load_game(void);

// ��ȡ������߷� 
uint32_t get_highest_score(void);

// ���±�����߷� 
void update_highest_score(void);

// �����ʷ��¼
void add_history(const char* name);

// �鿴��ʷ��¼
void list_history(void);

#endif//GAME2048_TOOLS_H
