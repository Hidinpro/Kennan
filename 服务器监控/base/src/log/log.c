#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include "log.h"

static const char *level_strings[] = {
	  "TRACE", 
	  "DEBUG", 
	  "INFO ", 
	  "WARN ", 
	  "ERROR", 
	  "FATAL"
};

static const char *level_colors[] = {
	  "\033[94m", 
	  "\033[36m", 
	  "\033[32m", 
	  "\033[33m", 
	  "\033[31m", 
	  "\033[35m"
};

static struct {
	uint8_t level;
	FILE* fps[LOG_FATAL+1];
	pthread_mutex_t lock;
}LOG;

void log_init(uint8_t level)
{
	LOG.level = level;
	LOG.fps[LOG_TRACE] = stdout;
	LOG.fps[LOG_DEBUG] = stdout;
	LOG.fps[LOG_INFO]  = stdout;
	LOG.fps[LOG_WARN]  = stderr;
	LOG.fps[LOG_ERROR] = stderr;
	LOG.fps[LOG_FATAL] = stderr;
	pthread_mutex_init(&LOG.lock,NULL);
}

void log_set_fp(uint8_t level,FILE* fp)
{
	LOG.fps[level] = fp;
}

const char* current_time(void)
{
	static char buf[20];
	time_t timer = time(NULL);
	struct tm* tm = localtime(&timer);
	strftime(buf,sizeof(buf),"%Y-%m-%d %H:%M:%S",tm);
	return buf;
}

void log_log(uint8_t level, const char *file, uint32_t line, const char *fmt, ...)
{
	if(level < LOG.level)
		return;

	pthread_mutex_lock(&LOG.lock);
	
	fprintf(LOG.fps[level],"%s%s %s %s:%d: ",
		level_colors[level],
		current_time(),
		level_strings[level],
		file,line);

	va_list args;
	va_start(args, fmt);
	vfprintf(LOG.fps[level],fmt,args);
	
	fprintf(LOG.fps[level],"\033[00m");

	pthread_mutex_unlock(&LOG.lock);
}
