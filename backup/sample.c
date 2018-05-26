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
char *getJson(void);
//extern devDataTable *g_devDataTab;
//extern DATAS_BUFF_T   comBuff0;
extern MQTT_SENT_BUFF_T mqBuff;
int *pAlmOidIdx;
int *pReadOidIdx;
INT32U g_reportTimeCnt=0;
char g_currentTime[50]={0};
static char  sMqNewpacketFlag=0;
char tmp[50] ={0};
char str[1500]={0};
void *sampleData_treat(void)
{
	data_classification();

	while(1)
	{

			g_reportTimeCnt++;
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
		//if((g_reportTimeCnt%g_devDataTab[i].upSentPeriod)!=0)continue;
		if(g_devDataTab[i].belongToOid>10)continue;
		formJsonPacket(i);
		//printf("dataReport :g_devDataTab[i].oid=:%d\n",g_devDataTab[i].oid);
	}
}
void formJsonPacket(int idx)
{
	cJSON *jsonRoot=NULL;
	cJSON *js_data =NULL;
	jsonRoot=cJSON_CreateObject();
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
	char *p = getJson();
	if(NULL == p)
	{
		cJSON_Delete(jsonRoot);
		return ;
	}
	strcpy(str,p);
    if(p)free(p);
	cJSON_Delete(jsonRoot);
	return;
	char *s =cJSON_PrintUnformatted(jsonRoot);
	//if(jsonRoot)cJSON_Delete(jsonRoot);
	if(s)
	{
		printf("%s\n",s);
		strcpy(str,s);

		free(s);
	}
	if(jsonRoot)cJSON_Delete(jsonRoot);
	//printf("int datalen=%d,%s\n",strlen(str),str);
	pthread_mutex_lock(&mqBuff.lock);
	if(strlen(str)>0)
	{
		mq_circleBuff_WritePacket(str,strlen(str),MQTP_REPORT);
		mqBuff.packetSum++;
		sMqNewpacketFlag=1;
	}
	pthread_mutex_unlock(&mqBuff.lock);


}

//void formJsonPacket(int idx)
//{
//	cJSON *jsonRoot=NULL;
//	jsonRoot=cJSON_CreateObject();
//
//	for(int j=0;j<1400;j++)str[j]=0x31;
//	str[1400]=0;
//	cJSON_AddStringToObject(jsonRoot, "test",str);
//    char *s = cJSON_PrintUnformatted(jsonRoot);
//
//	pthread_mutex_lock(&mqBuff.lock);
//	if(strlen(s)>0)
//	{
//		mq_circleBuff_WritePacket(str,strlen(str),MQTP_REPORT);
//		mqBuff.packetSum++;
//		sMqNewpacketFlag=1;
//		 free(s);
//	}
//	if(jsonRoot)cJSON_Delete(jsonRoot);
//	pthread_mutex_unlock(&mqBuff.lock);
//
//}

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
char *getJson(void)
{
    cJSON *pJsonRoot  = NULL;
    cJSON *pIntArray  = NULL;
    cJSON *pCommArray = NULL;
    cJSON *pSubJson   = NULL;
    char   *p          = NULL;
    int intarr[5] = {0, 1, 2, 3, 4}; //整数数组

    pJsonRoot = cJSON_CreateObject(); //创建JSON对象

    if(NULL == pJsonRoot)
    {
        //error happend here
        return NULL;
    }

    cJSON_AddStringToObject(pJsonRoot, "hello", "hello world"); //添加一个值为字符串的键值对"hello"："hello world"
    cJSON_AddNumberToObject(pJsonRoot, "number", 10010);        //添加一个值为数值的键值对"number"： 10010
    cJSON_AddBoolToObject(pJsonRoot, "bool", 1);                //添加一个值为布尔的键值对"bool"： 1



    pSubJson = cJSON_CreateObject();     //创建另一个json对象作为子对象

    if(NULL == pSubJson)
    {
        // create object faild, exit
        cJSON_Delete(pJsonRoot);         //删除pJsonRoot 及其子对象
        return NULL;
    }

    cJSON_AddStringToObject(pSubJson, "subjsonobj", "a sub json string"); //添加一个值为布尔的键值对
    cJSON_AddItemToObject(pJsonRoot, "subobj", pSubJson);                   //将对象pSubJson添加到pJsonRoot中,成为键值对 "subobj"：pSubJson


    //数值数组
    pIntArray = cJSON_CreateIntArray(intarr, 5); //为intarr创建一个数值数组对象，
    cJSON_AddItemToObject(pJsonRoot, "IntArr", pIntArray);  //将对象pIntArray添加到pJsonRoot中,成为键值对 " IntArr "：pIntArray


    //通用数组

    pCommArray = cJSON_CreateArray();//创建数组对象
    //cJSON_AddItemToArray(cJSON *array, cJSON *item);//向数组中添加对象
    cJSON_AddNumberToObject(pCommArray, "num", 2);             //向数组中添加数值cJSON_AddNumberToObject(object,name,n)
    cJSON_AddStringToObject(pCommArray, "str", "hopeview");    //向对象中添加字符串 cJSON_AddStringToObject(object,name,s)


    cJSON_AddItemToObject(pJsonRoot, "pCommArray ", pCommArray);  //将对象pCommArray添加到pJsonRoot中,成为键值对 " pCommArray "：pCommArray


    p = cJSON_Print(pJsonRoot);   //json对象转为json字符串用于传输

    // else use :
    // char * p = cJSON_PrintUnformatted(pJsonRoot);
    if(NULL == p)
    {
        //convert json list to string faild, exit
        //because sub json pSubJson han been add to pJsonRoot, so just delete pJsonRoot, if you also delete pSubJson, it will coredump, and error is : double free
        cJSON_Delete(pJsonRoot); //删除pJsonRoot 及其子对象
        return NULL;
    }

    //free(p);

    cJSON_Delete(pJsonRoot); //释放json对象
    printf("myJson is:%s",p);
    return p;               //返回json字符串，注意外面用完p要记得释放空间。//free(p);
}
