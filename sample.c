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
#include "cJSON.h"

//extern devDataTable *g_devDataTab;
//extern DATAS_BUFF_T   comBuff0;
extern MQTT_SENT_BUFF_T mqBuff;
int *pAlmOidIdx;
int *pReadOidIdx;
INT32U g_reportTimeCnt=0;
char g_currentTime[50]={0};
static char  sMqNewpacketFlag=0;
char tmp[50] ={0};

void *sampleData_treat(void)
{
	int i;


	data_classification();

	while(1)
	{

			g_reportTimeCnt=g_reportTimeCnt+100;
			data4Test();
			sleep(1);
			dataInt2String();
			dataReport_treat();
			pthread_mutex_lock(&mqBuff.lock);
			if(sMqNewpacketFlag !=0)
			{
				pthread_cond_signal(&mqBuff.newPacketFlag);
				sMqNewpacketFlag =0;
			}
			pthread_mutex_unlock(&mqBuff.lock);
	}
}
void data_classification(void)
{
	int m=0;
	int n=0;

	for(int i=0;i<g_tabLen-1;i++)
	{
		if(g_devDataTab[i].dataOption == ALM_20)m++;
	}
	pAlmOidIdx= (int *)malloc(m+1);
	m =0;
	for(int i=0;i<g_tabLen-1;i++)
	{
		if(g_devDataTab[i].dataOption == ALM_20)
		{
			pAlmOidIdx[m++]=i;
		}
	}
	pAlmOidIdx[m]=0xffff;
}

void dataReport_treat(void)
{
	getTime(&g_currentTime[0]);

	for(int i=0;i<g_tabLen-1;i++)
	{
		if(g_devDataTab[i].upSentPeriod ==0)continue;
		if((g_reportTimeCnt%g_devDataTab[i].upSentPeriod)!=0)continue;
		if(g_devDataTab[i].belongToOid>10)continue;
		formJsonPacket(i);
		//printf("dataReport :g_devDataTab[i].oid=:%d\n",g_devDataTab[i].oid);
	}
}
void formJsonPacket(int idx)
{

//	double dValue;
	cJSON *jsonRoot=NULL;
	cJSON *js_data =NULL;
//	dValue =(double)g_devDataTab[idx].valueInt/g_devDataTab[idx].radio;
//	gcvt(dValue,8,g_devDataTab[idx].valueString);
	//sprintf(g_devDataTab[idx].valueString, "%08.2f", dValue);
	//printf("oid =%d;valueInt=%d;valueString=%s\n",g_devDataTab[idx].oid,g_devDataTab[idx].valueInt,g_devDataTab[idx].valueString);

	jsonRoot=cJSON_CreateObject();
	//g_mqComVer ="v1.0";
	cJSON_AddStringToObject(jsonRoot,"version",g_mqComVer);
	cJSON_AddStringToObject(jsonRoot, "type",g_devDataTab[idx].ssType);
	strcat(tmp,g_mqComId);
	strcat(tmp,g_devDataTab[idx].ssDevId);
	cJSON_AddStringToObject(jsonRoot, "id",tmp);
	tmp[0]=0;
	cJSON_AddStringToObject(jsonRoot, "time",g_currentTime);
	cJSON_AddItemToObject(jsonRoot, "data", js_data=cJSON_CreateObject());
	if(g_devDataTab[idx].belongToOid !=1)
	{
		cJSON_AddStringToObject(js_data, g_devDataTab[idx].ssDataType,g_devDataTab[idx].valueString);
	}
	else
	{
		cJSON_AddStringToObject(js_data, g_devDataTab[idx].ssDataType,g_devDataTab[idx].valueString);
		for(int i=0;i<g_tabLen-1;i++)
		{
			if(g_devDataTab[i].upSentPeriod ==0)continue;
			if(g_devDataTab[idx].oid ==g_devDataTab[i].belongToOid )
			{
				cJSON_AddStringToObject(js_data, g_devDataTab[i].ssDataType,g_devDataTab[i].valueString);
			}
		}
}
	char *s =NULL;
	s =(char *) cJSON_PrintUnformatted(jsonRoot);
	//printf("int datalen=%d,%s\n",strlen(s),s);
	pthread_mutex_lock(&mqBuff.lock);
	mq_circleBuff_WritePacket(s,strlen(s),DTU2MQTPA);
	mqBuff.packetSum++;
	sMqNewpacketFlag=1;
	pthread_cond_signal(&mqBuff.newPacketFlag);
	pthread_mutex_unlock(&mqBuff.lock);

    cJSON_Delete(jsonRoot);
	//printf(cJSON_Print(jsonTmp));
	//printf("\n");

}


void  getTime(char *temp)
{
	struct   tm     *ptm;
	long       ts;
	int         y,m,d,h,n,s;
	char str[50]= {0};
	char strTemp[10]= {0};

	ts   =   time(NULL);
	ptm   =   localtime(&ts);
	y   =   ptm-> tm_year+1900;         //年
	m   =   ptm-> tm_mon+1;             //月
	d   =   ptm-> tm_mday;              //日
	h   =   ptm-> tm_hour;              //时
	n   =   ptm-> tm_min;               //分
	s   =   ptm-> tm_sec;               //秒
	sprintf(strTemp, "%d", y);
	strcat(str,strTemp);
	strcat(str,"-");
	if(m<10)strcat(str,"0");
	sprintf(strTemp, "%d", m);
	strcat(str,strTemp);
	strcat(str,"-");
	if(d<10)strcat(str,"0");
	sprintf(strTemp, "%d", d);
	strcat(str,strTemp);
	strcat(str," ");
	if(h<10)strcat(str,"0");
	sprintf(strTemp, "%d", h);
	strcat(str,strTemp);
	strcat(str,":");
	if(n<10)strcat(str,"0");
	sprintf(strTemp, "%d", n);
	strcat(str,strTemp);
	strcat(str,":");
	if(s<10)strcat(str,"0");
	sprintf(strTemp, "%d", s);
	strcat(str,strTemp);
	strcpy(temp,str);
}
void dataInt2String(void)
{
	double dValue;

	for(int i=0;i<g_tabLen-1;i++)
	{
		dValue =(double)g_devDataTab[i].valueInt/g_devDataTab[i].radio;
		gcvt(dValue,8,g_devDataTab[i].valueString);
	//	printf("g_devDataTab[i].valueString=%s\n",g_devDataTab[i].valueString);
	}
}
void data4Test(void)
{
	for(int i=0;i<g_tabLen-1;i++)
	{
		 if(g_devDataTab[i].dataOption !=ALM_20 && g_devDataTab[i].dataOption !=SET_1 )
			g_devDataTab[i].valueInt =i;
	}
}

