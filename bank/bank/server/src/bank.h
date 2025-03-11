#ifndef BANK_H
#define BANK_H

#define MSG_KEY 	ftok("../..",1)
#define LOG_PATH 	"../log/log.txt"
#define ACC_PATH	"../acc/"
#define BANK_PATH	"../acc/.bank.data"

#define	M_CREATE	1 	// 开户
#define M_DESTROY	2 	// 销户
#define M_LOGIN		3 	// 登录
#define M_UNLOCK	4 	// 解锁

#define M_SAVE		5 	// 存钱
#define M_TAKE		6 	// 取钱
#define M_TRANSF	7 	// 转账
#define M_QUERY		8 	// 查询
#define M_MODIFY	9	// 改密码


#define LOCK_CNT 	3	// 密码错误次数 >= LOCK_CNT 帐号锁定

typedef struct//帐户信息结构体
{
	char 	bank[11];	// 卡号 
	char 	card[19];	// 证件号
	char 	name[20];	// 姓名
	char 	passwd[7];	// 密码
	float	balance;	// 金额
	char 	lock;		// 是否锁定
}Acc;

typedef struct//客户端到服务器端的结构体
{
	long 	type;		//	消息类型
	Acc 	acc;		//	帐户信息
	pid_t 	pid;		//	客户端进程号
}Msgctos;

typedef struct//服务器端到客户端的结构体
{
	long 	type;		// 消息类型
	char 	str[255];	// 提示信息
}Msgstoc;

#endif//BANK_H
