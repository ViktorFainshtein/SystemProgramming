#include <stdio.h>
#include "test.h"

int x = 5;

void PrintGlobalVariableAddress()
{
	printf("%p\n", &x);
}
