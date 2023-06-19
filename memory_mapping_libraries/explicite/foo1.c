#include "global.h"
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int global_var1 = 1; /* data */
int global_var2 = 2; /* data */
int global_var3 = 3; /* data */

static int static_global_var1 = 1;
static int static_global_var2 = 2;
static int static_global_var3 = 3;

void Foo()
{

}

static void StaticFunction()
{
	
}

void ExternFunction(int functions_arg_1,float functions_arg_2,double functions_arg_3)
{
	
	const int const_local = 5;
	const int* p_const_local = &const_local;
	const int non_const_local = 5;
	const int* p_non_const_local = &non_const_local;
	    
	static int static_local_var1 = 111; /* data */
	static int static_local_var2 = 222; /* data */
	static int static_local_var3 = 333; /* data */
	    
	float* p_arg2 = &functions_arg_2;
	int* p_arg1 = &functions_arg_1;
	double* p_arg3 = &functions_arg_3;
	    
	void(*p_extern_function)(int,float,double) = ExternFunction;
	void(*p_static_function)() = StaticFunction;
	    
	int* p_heap_var = (int*)malloc(sizeof(int));
	char* p_heap_var2 = (char*)malloc(sizeof(p_heap_var2)); /* Check allignment */
	short* p_heap_var3 = (short*)malloc(sizeof(short)); /* Check allignment */    
	    
	int* p_global_var1 = &global_var1;
	int* p_global_var2 = &global_var2;
	int* p_global_var3 = &global_var3;
	    
	int* p_static_local_var1 = &static_local_var1;
	int* p_static_local_var2 = &static_local_var2;
	int* p_static_local_var3 = &static_local_var3;

	int* p_static_global_var1 = &static_global_var1;
	int* p_static_global_var2 = &static_global_var2;
	int* p_static_global_var3 = &static_global_var3;
	
	char* string_literal = "abc";
	
	printf("global_var1: %p\n", &global_var1);
	printf("global_var2: %p\n", &global_var2);
	printf("global_var3: %p\n\n\n", &global_var3);
	
	printf("static_global_var1: %p\n", &static_global_var1);
	printf("static_global_var2: %p\n", &static_global_var2);
	printf("static_global_var3: %p\n\n\n", &static_global_var3);
	
	printf("const_local: %p\n", &const_local);
	printf("p_const_local: %p\n\n\n", &p_const_local);

	printf("p_arg1: %p\n", p_arg1);
	printf("p_arg2: %p\n", p_arg2);
	printf("p_arg3: %p\n\n\n", p_arg3);
	
	printf("static_local_var1: %p\n", &static_local_var1);
	printf("static_local_var2: %p\n", &static_local_var2);
	printf("static_local_var3: %p\n\n\n", &static_local_var3);
	
	printf("p_heap_var: %p\n", p_heap_var);
	printf("p_global_var1: %p\n", p_global_var1);
	printf("p_static_local_var1: %p\n\n\n", p_static_local_var1);
	
	printf("p_extern_function: %p\n", p_extern_function);
	printf("p_static_function: %p\n\n\n", p_static_function);
	
	printf("string_literal: %p\n", &string_literal);
}
