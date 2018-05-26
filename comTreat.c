#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "public.h"


void *comThread(void)
 {

	 while (1)
	 {
		 pthread_mutex_lock(&RecvBuff4treat.lock);
		 pthread_cond_wait(&RecvBuff4treat.newPacketFlag, &RecvBuff4treat.lock);
		 printf("-----enter--com_Treat\n");
		 pthread_mutex_unlock(&RecvBuff4treat.lock);

	 }
	 return NULL;
 }
