#include <dlfcn.h>

int main()
{
	/*
        void *handle1;
        void *handle2;
	void (*foo1)();
	void (*foo2)();	
	
        handle1 = dlopen ("libtest1.so", RTLD_LAZY);
        handle2 = dlopen ("libtest2.so", RTLD_LAZY);

        foo1 = dlsym(handle1, "Foo");	
       	foo2 = dlsym(handle2, "Foo2");
	
	(*foo1)();
	(*foo2)();
        
        dlclose(handle1);
        dlclose(handle2);
	*/
	Foo();
	Foo2();
	PrintGlobalVariableAddress();

	return 0;
}
