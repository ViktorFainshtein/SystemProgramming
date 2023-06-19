#include <stdio.h>	/*printf*/
#include <stdlib.h>	/*malloc free*/

int global_var;								/*extern global var*/
static int static_global_var;						/*static global var*/
const int const_var;							/*const global var*/
static const int const_static_global_var;					/*const static gloval var*/


static int StaticFunc()								/*static function*/
{
	int local_static = 1;							/*local var*/
	const int const_local_static = 2;						/*const local var*/
	static int static_local_static = 3;					/*static local var*/
	static const int const_static_local_static = 4;				/*const static local var*/
	char *static_str = "12345";							/*uncangable string*/
	char *static_ch = NULL;							/*dynamic var declaration*/	
	static_ch = (char *)malloc(sizeof(char)+1);					/*dynamic var allocation*/
	*static_ch = '6';
	
	free(static_ch);
	return 0;
}

int ExternFunc()								/*extern function*/
{
	int local_extern = 1;							/*local var*/
	const int const_local_extern = 2;						/*const local var*/
	static int static_local_extern = 3;					/*static local var*/
	static const int const_static_local_extern = 4;				/*const static local var*/
	char *extern_str = "12345";							/*uncangable string*/	
	char *extern_ch = NULL;							/*dynamic var declaration*/	
	extern_ch = (char *)malloc(sizeof(char)+1);					/*dynamic var allocation*/
	*extern_ch = '6';
	
	free(extern_ch);
	return 0;
}

int RecursiveFunc(int res)							/*recursive function*/
{
	int local_rec = 1;							/*local var*/
	const int const_local_rec = 2;						/*const local var*/
	static int static_local_rec = 3;					/*static local var*/
	static const int const_static_local_var = 4;				/*const static local var*/
	char *rec_str = "12345";							/*uncangable string*/	
	char *rec_ch = NULL;							/*dynamic var declaration*/	
	rec_ch = (char *)malloc(sizeof(char)+1);					/*dynamic var allocation*/
	*rec_ch = '6';
	free(rec_ch);
	
	if(res == 0)								/*stoping expresion for recursion*/
	{
		return 1;
	}
	
	return RecursiveFunc(local_rec-1);
}

int main(int argc, char *argv[], char *envp[])					/*main reciving arguments*/
{
	StaticFunc();
	
	ExternFunc();
	
	RecursiveFunc(1);
	
	
	return 0;
}
