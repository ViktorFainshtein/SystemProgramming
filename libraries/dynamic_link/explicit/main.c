

#include <dlfcn.h>


int main()
{
	int var = 10;
        void *handle;
	void (*foo)(int);
	
        handle = dlopen ("libfoo.so", RTLD_LAZY);

        foo = dlsym(handle, "Foo");
	
	foo(10);
        
        dlclose(handle);
	
	
	
	
	
	

	return 0;
}
