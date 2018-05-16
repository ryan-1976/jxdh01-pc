#include <stdio.h>
#include "cJSON.h"
#include "sqlite3.h"
#include <stdlib.h> //exit
char * makeJson()
{
    cJSON * pRoot = NULL;
    cJSON * pSub_1 = NULL;
    cJSON * pSub_2 = NULL;

    if((pRoot = cJSON_CreateObject()) == NULL)
    {
        return NULL;
    }
    if((pSub_1 = cJSON_CreateObject()) == NULL)
    {
        return NULL;
    }
    if((pSub_2 = cJSON_CreateObject()) == NULL)
    {
        return NULL;
    }
    cJSON_AddStringToObject(pSub_2, "cStr", "ccccccc");

    cJSON_AddStringToObject(pSub_1, "bStr", "bbbbbbb");
    cJSON_AddItemToObject(pSub_1, "subobject_2", pSub_2);

    cJSON_AddStringToObject(pRoot, "aStr", "aaaaaaa");
    cJSON_AddItemToObject(pRoot, "subobject_1", pSub_1);
    cJSON_AddStringToObject(pRoot, "xStr", "xxxxxxx");

    //cJSON_PrintUnformatted : make json string for Unformatted
    //char * pJson = cJSON_PrintUnformatted(pRoot);

    char * pJson = cJSON_Print(pRoot);
    if(NULL == pJson)
    {
        cJSON_Delete(pRoot);
        return NULL;
    }
    return pJson;
}

int parseJson(const char * pJson)
{
    if(NULL == pJson)
    {
        return 1;
    }
    cJSON * pRoot = cJSON_Parse(pJson);
    if(NULL == pRoot)
    {
        return 2;
    }
    cJSON * pSub_1 = cJSON_GetObjectItem(pRoot, "aStr");
    if(NULL == pSub_1)
    {
        cJSON_Delete(pRoot);
        return 3;
    }
    printf("get aStr : [%s]\n", pSub_1->valuestring);
    pSub_1 = cJSON_GetObjectItem(pRoot, "xStr");
    if(NULL == pSub_1)
    {
        cJSON_Delete(pRoot);
        return 4;
    }
    printf("get xStr : [%s]\n", pSub_1->valuestring);
    pSub_1 = cJSON_GetObjectItem(pRoot, "subobject_1");
    if(NULL == pSub_1)
    {
        cJSON_Delete(pRoot);
        return 5;
    }
    printf("get Sub Obj 1\n");
    cJSON * pSub_2 = cJSON_GetObjectItem(pSub_1, "bStr");
    if(NULL == pSub_2)
    {
        cJSON_Delete(pRoot);
        return 6;
    }
    printf("get bStr : [%s]\n", pSub_2->valuestring);
    pSub_2 = cJSON_GetObjectItem(pSub_1, "subobject_2");
    if(NULL == pSub_2)
    {
        cJSON_Delete(pRoot);
        return 7;
    }
    printf("get Obj 2\n");
    cJSON * pStr = cJSON_GetObjectItem(pSub_2, "cStr");
    if(NULL == pStr)
    {
        cJSON_Delete(pRoot);
        return 8;
    }
    printf("get cStr : [%s]\n", pStr->valuestring);

    cJSON_Delete(pRoot);
    return 0;
}

int main()
{



    return 0;
}
