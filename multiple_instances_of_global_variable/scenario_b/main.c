#include <stdio.h>
#include <dlfcn.h>
#include "test.h"



int main()
{
	void *handle = NULL;
	void *foo = NULL;
	
	x = 10;	
		
	printf("before dl open: %d\n", x);
	printf("before dl open: %p\n", &x);
	
	handle = dlopen ("libtest.so", RTLD_LAZY);
	foo = dlsym(handle, "x");
	
	printf("before dl open foo: %d\n", *(int *)foo);
	printf("before dl open foo: %p\n", foo);
	
	dlclose(handle);
	
	printf("before dl open X: %d\n", x);
	printf("before dl open X: %p\n", &x);
	

	return 0;
}
