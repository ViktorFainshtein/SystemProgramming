#include <stdio.h>
#include "global.h"

int main(void)
{
	PrintGlobalVariableAddress();
	Foo();
	Foo2();
	ExternFunction(1, 2.0, 4.0);
	
	return(0);
}
