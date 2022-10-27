#include <monitor.h>
#include <unistd.h>
#include <pthread.h>

void* monitored_run(void* arg)
{
	wait_monitored();
}

void* viewers_run(void* arg)
{
	wait_viewers();
}

int main(int argc,const char* argv[])
{
	init_monitor(10);
	
	pthread_t monitored_tid;
	pthread_create(&monitored_tid,NULL,monitored_run,NULL);
	
	pthread_t viewers_tid;
	pthread_create(&viewers_tid,NULL,viewers_run,NULL);
	
	for(;;)
	{
		log_monitor();
		sleep(5);
	}
}
