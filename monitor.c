#include <stdio.h>
#include "cJSON.h"
#include "sqlite3.h"
#include <stdlib.h> //exit
#include <pthread.h>
#include "circlebuff.h"

extern void * mqtt_sub_treat(int argc, char* argv[]);
extern void * mqtt_pub_treat(int argc, char* argv[]);
extern void * msgDisPatcherThread(void);
extern void * sampleData_treat(void);
//extern void * sqlite_treat(void);
//extern void * bluetooth_treat(void);
extern void G_Buff_init(void);
extern void  json_init();
extern void  sqTable_init();;
int main()
{

	pthread_t th_a, th_b,th_c,th_d,th_e,th_f;
	void *retval;

	json_init();
	sqTable_init();
	G_Buff_init();
	//tabProc();
	//pthread_create(&th_d, NULL, (void *(*)(void *))sqlite_treat, 0);
	pthread_create(&th_a, NULL, (void *(*)(void *))msgDisPatcherThread, 0);
	pthread_create(&th_b, NULL, (void *(*)(void *))mqtt_sub_treat, 0);
	pthread_create(&th_c, NULL, (void *(*)(void *))mqtt_pub_treat, 0);
	pthread_create(&th_e, NULL, (void *(*)(void *))sampleData_treat, 0);
	//pthread_create(&th_f, NULL, (void *(*)(void *))bluetooth_treat, 0);

	//pthread_join(th_d, &retval);
	pthread_join(th_a, &retval);
	pthread_join(th_b, &retval);
	pthread_join(th_c, &retval);
	pthread_join(th_e, &retval);
	//pthread_join(th_f, &retval);
	return 0;

    return 0;
}
