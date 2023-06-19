#include <stdio.h> /* printf */
#include "global.h"

int x = 7;

void PrintGlobalVariableAddress()
{
	printf("%p\n", &x);	
}


