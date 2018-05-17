#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sqlite3.h"
#include "circlebuff.h"

extern DATAS_BUFF_T   comBuff0; 
extern  g_devDataTab[];
void *sampleData_treat(void)
{
	int i;

	while(1)
	{
		printf("---enter ---sampleData_treat----------\n");
//		pthread_mutex_lock(&comBuff0.lock);
//		spi2MqtttPacket();
//		pthread_cond_signal(&comBuff0.newPacketFlag);
//		pthread_mutex_unlock(&comBuff0.lock);
		sleep(2);//delay 1s
	}
}
