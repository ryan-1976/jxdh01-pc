#include <stdio.h>
#include "sqlite3.h"
#include <stdlib.h>
#include "public.h"
#include <string.h>

devDataTable *g_devDataTab;
int  g_tabLen=0;

void sqTable_init(void)
{
	sqlite3 *pdb = NULL;

	char** pResult;
	int nRow,nCol,nResult;
	char* errmsg;
	//nResult = sqlite3_open_v2("jxdh01-v01.db",&pdb,SQLITE_OPEN_READWRITE,NULL);
	nResult = sqlite3_open_v2("/home/ryan/share2win/jxdh01-v02.db",&pdb,SQLITE_OPEN_READWRITE,NULL);
    if(nResult)
    {
            fprintf(stderr,"can't open database:%s\n",sqlite3_errmsg(pdb));
            sqlite3_close(pdb);
            exit(EXIT_FAILURE);
    }
    printf("open db ok\n");
    char * strSql = "select * from devData";
    nResult = sqlite3_get_table(pdb,strSql,&pResult,&nRow,&nCol,&errmsg);
    if (nResult != SQLITE_OK)
  {
      sqlite3_close(pdb);

      sqlite3_free(errmsg);
      return ;
  }
    printf("sqlite3_get_table ok\n");

  int nIndex = nCol;

  g_tabLen = nRow-1;


  if ((g_devDataTab = (devDataTable *)malloc(g_tabLen * sizeof(devDataTable))) == NULL)    {

         printf("malloc g_devDataTab error\n");
         exit(EXIT_FAILURE);
     }

  for(int i=0;i<nRow-1;i++)
  {
   //   for(int j=0;j<nCol;j++)
      {

    	  g_devDataTab[i].oid =atoi(pResult[nIndex]);
    	  strcpy(g_devDataTab[i].oidName,pResult[nIndex+1]);
    	  strcpy(g_devDataTab[i].ssType,pResult[nIndex+2]);
    	  strcpy(g_devDataTab[i].ssTypeCnName,pResult[nIndex+3]);
    	  strcpy(g_devDataTab[i].ssDataType,pResult[nIndex+4]);
    	  strcpy(g_devDataTab[i].ssDevId,pResult[nIndex+5]);

    	  g_devDataTab[i].valueInt = atoi(pResult[nIndex+6]);
    	  g_devDataTab[i].valueIntPre =g_devDataTab[i].valueInt;
    	  g_devDataTab[i].upSentPeriod =atoi( pResult[nIndex+7]);
    	  g_devDataTab[i].dataOption =atoi( pResult[nIndex+8]);

    	  g_devDataTab[i].belongToOid = atoi(pResult[nIndex+9]);
    	  g_devDataTab[i].belongOidIdx = atoi(pResult[nIndex+10]);
    	  g_devDataTab[i].mbDataLen = atoi(pResult[nIndex+11]);

    	  g_devDataTab[i].mbDataType = atoi(pResult[nIndex+12]);
    	  g_devDataTab[i].radio = atoi(pResult[nIndex+13]);
    	  strcpy(g_devDataTab[i].resouceMap,pResult[nIndex+14]);
   	      g_devDataTab[i].mbAddr = atoi(pResult[nIndex+15]);
   	      nIndex=nIndex+nCol;
//
 // 	        printf("g_devDataTab[i].oid=:%d\n",g_devDataTab[i].oid);
//			//printf("g_devDataTab[i].oid);
//			printf(":");printf(g_devDataTab[i].oidName);
//			printf(":");printf(g_devDataTab[i].treatOption);
//			printf("\n");


      }
  }

  sqlite3_free_table(pResult);
  //printf(strOut);
  sqlite3_close(pdb);
    printf("open ok\n");

  // exit(EXIT_SUCCESS);

}
