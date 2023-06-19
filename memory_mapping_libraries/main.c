#include <stdio.h>
#include <dlfcn.h>
#include "global.h"

int main(void)
{
	void *handle1;
	void *handle2;
	void (*p_Foo)();
	void (*p_Foo2)();
	void (*p_ExternFunction)(int, float, double);
	
	
	handle1 = dlopen("./libfoo1.so", RTLD_LAZY);
	
	if(!handle1)
	{
		return 1;
	}
	handle2 = dlopen("./libfoo2.so", RTLD_LAZY);
	
	if(!handle2)
	{
		return 1;
	}
	
	*(void**)&p_Foo = dlsym(handle1, "Foo");
	*(void**)&p_ExternFunction = dlsym(handle1, "ExternFunction");
	*(void**)&p_Foo2 = dlsym(handle2, "Foo2");
	
	if(!p_Foo || !p_Foo2 ||!p_ExternFunction )
	{
		printf("faillledd");
		return(1);
	}
	
	PrintGlobalVariableAddress();
	(*p_Foo)();
	(*p_Foo2)();
	(*p_ExternFunction)(5,2.0,10.0);
	
	dlclose(handle1);
	dlclose(handle2);
	
	
	
	return(0);
}
