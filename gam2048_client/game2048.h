#ifndef GAME2048_H
#define GAME2048_H

#include <stdint.h>
#include <stdbool.h>

extern uint16_t body[4][4];
extern uint32_t current_score;
extern uint32_t highest_score;
extern bool is_rand;

void init_game2048(void);

void start_game2048(void);

void exit_game2048(void);

#endif//GAME2048_H
