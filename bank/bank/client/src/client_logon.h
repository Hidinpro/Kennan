#ifndef CLIENT_LOGON_H
#define CLIENT_LOGON_H
#include "bank.h"

extern int msgfd;
extern int cli_fd;
extern Msgctos ctos;
extern Msgstoc stoc;

void logon(void);

#endif//CLIENT_LOGON_H
