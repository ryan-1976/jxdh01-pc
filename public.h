/*
*******************************************************************************
*                    Copyright (c) 2013, TQ System
*                             All Rights Reserved
*
* Purpose           : COM file
* File Name         : PUBLIC.H

* Revision history  :
    01a,2018/05/16,Ryan  -- Creation(written)

* Description       :

*******************************************************************************
*/
#include <pthread.h>

#ifndef _AP_PUBLIC_INCLUDED_
#define _AP_PUBLIC_INCLUDED_

//#define   TZigbee_PACK_HEAD       0xA5
#define   MAX_BUFF_LEN            102400
#define   MQ_BUFF_LEN             20240
#define   RECV_BUFF_LEN           2024
#define   MAX_PACKET_BUFF_LEN     2100  
#define BUSY 1;
#define IDLE 0;

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                  
typedef signed   char  INT8S;                   
typedef unsigned short INT16U;                  
typedef signed   short INT16S;                 
typedef unsigned int   INT32U;                  
typedef signed   int   INT32S;                 

#define CDTU       0X01
#define MQTT       0X02
#define UDP        0X03
#define MODEM      0X04
#define SERIAL1    0X05

#define MQTPA2DTU    0X0201
#define DTU2MQTPA    0X0102
#define MQTPB2DTU    0X0301
#define DTU2MQTPB    0X0103
#define MQTPA        0X00
#define MQTPB        0X01

#define SET_0        0

#define READ_10      10
#define READ_11      11
#define READ_30      11
#define READ_100     100

#define ALM_20       20
#define SET_1        1


typedef struct
{
	volatile unsigned long int    readPos;
	volatile unsigned long int    writePos;
    unsigned char   data[MAX_BUFF_LEN];
	pthread_mutex_t lock;
	pthread_cond_t  newPacketFlag;
	
}DATAS_BUFF_T;
typedef struct
{
	volatile unsigned long int    readPos;
	volatile unsigned long int    writePos;
    unsigned char   data[MQ_BUFF_LEN];
    unsigned char   mqttTopicFlag;
    unsigned int    len;
	pthread_mutex_t lock;
	pthread_cond_t  newPacketFlag;
	unsigned int    packetSum;
}MQTT_SENT_BUFF_T;
typedef struct
{
    unsigned char   data[RECV_BUFF_LEN];
    unsigned char   protoltype;
    unsigned char   scrFlag;//data from where
    unsigned int    len;
	pthread_mutex_t lock;
	pthread_cond_t  newPacketFlag;
}RECV_BUFF_T;

typedef struct{
	int oid;
	int valueInt;
	int valueIntPre;
	int radio;
	int upSentPeriod;
	int belongToOid;
	int mbAddr;
	int dataOption;
	int belongOidIdx;
	int mbDataLen;
	int mbDataType;
	char ssDevId[5];
	char valueString[10];
	char ssType[30];
	char ssDataType[30];
	char resouceMap[30];
	char ssTypeCnName[50];
	char oidName[50];

}devDataTable;
extern int  get_oidIdx(int oid);
extern devDataTable *g_devDataTab;
extern int  g_tabLen;
extern char *g_mqComVer;
extern char *g_mqComId;
#endif

