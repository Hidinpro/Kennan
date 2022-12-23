#ifndef ADD_H
#define ADD_H

#define TIME 1.2
#define STUDENT_MAX 500    //学生的最大数量为500,操作员的最大数量为20
#define OPERATOR_MAX 20  
extern int operator_cnt;
extern int student_cnt;

typedef struct Student   //学生结构体
{
    char name[20];
    char student_id[10];
    char password[10];
}Student;


typedef struct Operator  //操作员结构体
{
    char password[7];
    int id;
}Operator;

Student student[STUDENT_MAX];
Operator operator[OPERATOR_MAX]; 

void add_operator(void);   			            //增加操作员


#endif//ADD_H