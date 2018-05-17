#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "public.h"


int  get_oidValue(int oid)
{
	for(int i=0;i<g_tabLen-1;i++)
	{
		if(g_devDataTab[i].oid == oid)
			return g_devDataTab[i].valueInt;
	}
    return -1;

}
int  get_oidIdx(int oid)
{
	for(int i=0;i<g_tabLen-1;i++)
	{
		if(g_devDataTab[i].oid == oid)
			return i;
	}
    return -1;

}
