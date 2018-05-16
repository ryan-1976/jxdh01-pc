#include <stdio.h>
#include "cJSON.h"
#include "sqlite3.h"
#include <stdlib.h> //exit


int main()
{
	json_init();
	sqTable_init();

    return 0;
}
