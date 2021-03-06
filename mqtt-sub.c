#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
#include "circlebuff.h"
#include <unistd.h>
#include "public.h"
//#define ADDRESS     "tcp://localhost:1883"
//#define ADDRESS     "tcp://192.168.3.101:1883"
//#define ADDRESS     "tcp://47.106.81.63:1883"
#define CLIENTID    "ryan-02"
//#define CLIENTID1   "11111111111122sub"
//#define TOPIC       "mqtt/31111111111111"

#define QOS         1
#define TIMEOUT     10000L

extern DATAS_BUFF_T   comBuff0; 

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;

    printf(" new mqtt Message arrived:");
    printf(" topic: %s", topicName);
	printf(" topicLen:%d\n ",message->payloadlen);

    payloadptr = message->payload;

	pthread_mutex_lock(&comBuff0.lock);

	AP_circleBuff_WritePacket(payloadptr++,message->payloadlen,MQTPA2DTU);
	pthread_cond_signal(&comBuff0.newPacketFlag);
	pthread_mutex_unlock(&comBuff0.lock);
	
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

void *mqtt_sub_treat(int argc, char* argv[])
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;
    int ch;

	//--------------------------------------------------------------
	//printf("-------enter mqtt_sub_treat-----------------\n");
    MQTTClient_create(&client, g_mqServer, CLIENTID,
    MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("sub start up Failed to connect, return code %d\n", rc);
        //exit(EXIT_FAILURE);
    }
    printf("Sub topic: %s\nfor clientId: %s using QoS%d\n\n", g_mqTopicCtrl, CLIENTID, QOS);
    MQTTClient_subscribe(client, g_mqTopicCtrl, QOS);


    do 
    {
		while(!MQTTClient_isConnected(client))
		{
			if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
			{
				printf(" sub retry to connect failure, return code %d\n", rc);
			}
			else
			{
				printf("Sub topic: %s\nfor clientId: %s using QoS%d\n\n", g_mqTopicCtrl, CLIENTID, QOS);
				MQTTClient_subscribe(client, g_mqTopicCtrl, QOS);

			}
			sleep(3);
			printf("-------sub wait to connected-----------------\n");
		};
		sleep(3);

    }while(1);

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    //return rc;
}
