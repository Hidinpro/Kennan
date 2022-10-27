#ifndef LOG_H
#define LOG_H
#include <stdio.h>
#include <stdint.h>

// 日志等级
#define LOG_TRACE	0	
#define LOG_DEBUG	1
#define LOG_INFO	2
#define LOG_WARN	3
#define LOG_ERROR	4
#define LOG_FATAL	5

#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void log_init(uint8_t level);
void log_set_fp(uint8_t level,FILE* fp);
void log_log(uint8_t level, const char *file, uint32_t line, const char *fmt, ...);

#endif//LOG_H
