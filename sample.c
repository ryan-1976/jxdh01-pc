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
#include "public.h"

//extern devDataTable *g_devDataTab;
extern DATAS_BUFF_T   comBuff0; 
int *pAlmOidIdx;
int *pReadOidIdx;
void *sampleData_treat(void)
{
	int i;


	data_classification();


	while(1)
	{
		printf("---enter ---sampleData_treat----------\n");

//		pthread_mutex_lock(&comBuff0.lock);
//		spi2MqtttPacket();
//		pthread_cond_signal(&comBuff0.newPacketFlag);
//		pthread_mutex_unlock(&comBuff0.lock);
		sleep(1);//delay 1s
	}
}
void data_classification(void)
{
	int m=0;
	int n=0;

	for(int i=0;i<g_tabLen-1;i++)
	{
		if(g_devDataTab[i].treatOption == ALM_20)m++;

	}
	pAlmOidIdx= (int *)malloc(m+1);
	m =0;
	for(int i=0;i<g_tabLen-1;i++)
	{
		printf("g_devDataTab[i].oid=:%d\n",(int)g_devDataTab[i].oid);
		if(g_devDataTab[i].treatOption == ALM_20)
		{
			pAlmOidIdx[m++]=i;
		}

	}
	pAlmOidIdx[cnt]=0xffff;
//	printf("pAlmOidIdx[0]=:%d\n",pAlmOidIdx[0]);
//	printf("pAlmOidIdx[1]=:%d\n",pAlmOidIdx[1]);
//	printf("pAlmOidIdx[2]=:%d\n",pAlmOidIdx[2]);
}

void dataReport_treat(void)
{


}


