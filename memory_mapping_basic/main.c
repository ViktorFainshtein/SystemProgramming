#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>


int global_var1 = 1;
int global_var2 = 2;
int global_var3 = 3;

/* Put this in .h file: */
static int static_global_var1 = 11;
static int static_global_var2 = 22;
static int static_global_var3 = 33;


static void StaticFunction()
{

}

extern void ExternFunction();

int MemoryMappingCode(int functions_arg_1,float functions_arg_2,double functions_arg_3)
{
    static int recursion_level = 1;

    int* p_arg1 = &functions_arg_1;
    (void)p_arg1;
    float* p_arg2 = &functions_arg_2;
    (void)p_arg2;
    double* p_arg3 = &functions_arg_3;
    (void)p_arg3;
    void(*p_static_function)() = StaticFunction;
    (void)p_static_function;
    void(*p_extern_function)() = ExternFunction;
    (void)p_extern_function;
    int* p_heap_var = (int*)malloc(sizeof(int));
    (void)p_heap_var;
    char* p_heap_var2 = (char*)malloc(sizeof(char)); /* Check allignment */
    (void)p_heap_var2;
    short* p_heap_var3 = (short*)malloc(sizeof(short)); /* Check allignment */    
    (void)p_heap_var3;
    const int const_local = 5;
    const int* p_const_local = &const_local;
    (void)p_const_local;
    const int non_const_local = 5;
    const int* p_non_const_local = &non_const_local;
    (void)p_non_const_local;
    int* p_global_var1 = &global_var1;
    (void)p_global_var1;
    int* p_global_var2 = &global_var2;
    (void)p_global_var2;
    int* p_global_var3 = &global_var3;
    (void)p_global_var3;    
    static int static_local_var1 = 111;
    static int static_local_var2 = 222;
    static int static_local_var3 = 333;
    int* p_static_local_var1 = &static_local_var1;
    (void)p_static_local_var1;
    int* p_static_local_var2 = &static_local_var2;
    (void)p_static_local_var2;
    int* p_static_local_var3 = &static_local_var3;
    (void)p_static_local_var3;        
    int* p_static_global_var1 = &static_global_var1;
    (void)p_static_global_var1;
    int* p_static_global_var2 = &static_global_var2;
    (void)p_static_global_var2;
    int* p_static_global_var3 = &static_global_var3;
    (void)p_static_global_var3;
    char* string_literal = "abc";
    (void)string_literal;

    free(p_heap_var);

    MemoryMappingCode(functions_arg_1+1,functions_arg_2+1,functions_arg_1+1);

    ++recursion_level;
    return recursion_level;
}

int main(int argc, char *argv[], char *envp[])
{
    char** p_command_line_arguments = argv;
    (void)p_command_line_arguments;
    char** p_environment_variables = envp;
    (void)p_environment_variables;
    (void)argc;

    MemoryMappingCode(1,1.0f,1.0);

    return 0;
}

