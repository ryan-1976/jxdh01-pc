#include <stdio.h>
#include "sqlite3.h"
#include <stdlib.h>
#include "public.h"

devDataTable g_devDataTab[100];
int  g_tabLen=0;
void sqTable_init(void)
{
	sqlite3 *pdb = NULL;

	char** pResult;
	int nRow,nCol,nResult;
	char* errmsg;

	nResult = sqlite3_open_v2("jxdh01-v01.db",&pdb,SQLITE_OPEN_READWRITE,NULL);
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

  for(int i=0;i<nRow-1;i++)
  {
   //   for(int j=0;j<nCol;j++)
      {
    	  g_devDataTab[i].oid = pResult[nIndex];
    	  strcpy(g_devDataTab[i].oidName,pResult[nIndex+1]);
    	  strcpy(g_devDataTab[i].ssType,pResult[nIndex+2]);
    	  strcpy(g_devDataTab[i].ssTypeCnName,pResult[nIndex+3]);
    	  strcpy(g_devDataTab[i].ssDataType,pResult[nIndex+4]);
    	  g_devDataTab[i].valueInt = pResult[nIndex+5];
    	  g_devDataTab[i].upSentPeriod = pResult[nIndex+7];
    	  g_devDataTab[i].treatOption = pResult[nIndex+8];

    	  g_devDataTab[i].belongToOid = pResult[nIndex+9];
    	  g_devDataTab[i].belongOidIdx = pResult[nIndex+10];
    	  g_devDataTab[i].mbDataLen = pResult[nIndex+11];

    	  g_devDataTab[i].mbDataType = pResult[nIndex+12];
    	  g_devDataTab[i].radio = pResult[nIndex+13];
    	  strcpy(g_devDataTab[i].resouceMap,pResult[nIndex+14]);
   	      g_devDataTab[i].mbAddr = pResult[nIndex+15];


//    	  printf(g_devDataTab[i].oid);
//    	  printf(":");
//    	  printf(g_devDataTab[i].oidName);
//    	  printf(":");
//
//    	  printf(g_devDataTab[i].ssType);
//    	  printf(":");
//    	  printf(g_devDataTab[i].upSentPeriod);

   	   printf(g_devDataTab[i].oid);	printf(":");
   	   printf(g_devDataTab[i].ssTypeCnName);
    	  printf("\n");
    	  nIndex=nIndex+nCol;
//    	  printf(pResult[j]);
//    	  printf(":");
//    	  printf(pResult[nIndex]);
//    	  printf("\n");
//    	  ++nIndex;
      }
  }

  sqlite3_free_table(pResult);
  //printf(strOut);
  sqlite3_close(pdb);
    printf("open ok\n");

   // exit(EXIT_SUCCESS);

}
