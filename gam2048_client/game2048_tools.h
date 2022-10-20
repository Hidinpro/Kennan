#ifndef GAME2048_TOOLS_H
#define GAME2048_TOOLS_H

// 随机生成一个整数 
void rand_num(void);

// 显示界面 
void show_body(void);

// 判断游戏是否结束 
bool is_gameover(void);

// 获取方向键
int get_cmd(void);

// 再一来局
void new_game(void);

// 存档
void save_game(void); 

void up_move(void);

void down_move(void);

void right_move(void);

void left_move(void);

// 存档
void save_game(void); 

// 加载存档 
void load_game(void);

// 获取本机最高分 
uint32_t get_highest_score(void);

// 更新本机最高分 
void update_highest_score(void);

// 添加历史记录
void add_history(const char* name);

// 查看历史记录
void list_history(void);

#endif//GAME2048_TOOLS_H
