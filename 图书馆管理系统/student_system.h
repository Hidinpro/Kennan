#ifndef STUDENT_SYSTEM_H
#define STUDENT_SYSTEM_H

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"

#define MAX_BOOKNUM 1000000
#define MAX_RECORD 10000
#define MAX_STUDENT 10000

// ==================================================================================

void enter(void);

void student_menu(void);

int student1_login(void);

int query_stuid(const char* id);

int _chinese_vague_search(char* str,char* target);

void book_vague_search(void);

void show_bookmsg(int i);

void reserved_book(int i);

int query_student_id_index(char* id);


void show_borrow_msg(int i);

void show_return_msg(int i);

void show_reserved_msg(int i);

void show_over_deadline_msg(int i);

#endif//STUDENT_SYSTEM_H