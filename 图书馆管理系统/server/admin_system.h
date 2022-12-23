#ifndef ADMIN_H
#define ADMIN_H

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


//管理员界面
void admin_menu(void);                  //管理员菜单
void admin_operator_menu(void);         //管理员管理后勤人员菜单  
void admin_student_menu(void);          //管理员管理学生菜单
void operator_choicechange_menu(void);  //选择哪一项更改
void menu(void);                        //管理员界面



//管理员系统

int login_operator(void);                       //操作员登录

int book_admin(void);                           //管理员登录

void admin(void);                               //管理员选择界面

void register_operator(void);                   //管理操作员界面

void add_operator(void);   			            //增加操作员

void show_person(void);                         //遍历后勤人员

void reduce_operator(void);  		            //选择输入操作员ID来进行删除
 
void change_operator(void);                     //修改操作员信息

void operator_id_change(int position);          //修改操作员姓名

void operator_passwd_change(int place);			//修改操作员的密码

void show_operator(void);     			        //遍历操作员


//学生管理系统   

int query_studentid(char* id); 		//用学生ID 查找学生

int student_login(void);                  //学生登录

void register_student(void);              //管理学生

void add_student(void);                   //增加新学生

void reduce_student(void);                //删除学生

void change_student(void);                //修改学生信息

void student_name_change(int position);   //修改姓名

void student_passwd_change(int place);	  //修改学生的密码

void show_student(void);                  // 遍历学生

#endif//ADMIN_H