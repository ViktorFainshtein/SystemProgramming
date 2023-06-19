#include <stdio.h>
#include <dlfcn.h>
#include "test.h"



int main()
{
	int x = 10;
	void *handle = NULL;
	void *foo = NULL;
	
	printf("%d\n", x);
	printf("%p\n", &x);
	
	handle = dlopen ("libtest.so", RTLD_LAZY);
	foo = dlsym(handle, "x");
	
	printf("%d\n", *(int *)foo);
	printf("%p\n", foo);
	
	dlclose(handle);
	
	printf("%d\n", x);
	printf("%p\n", &x);
	

	return 0;
}
