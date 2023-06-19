#include <stdio.h>
#include <stdlib.h>
#include "fsa.h"

int main()
{
	size_t *ptr = malloc(100);
	
	fsa_t *fsa = NULL;
	
	size_t *node1 = NULL;
	size_t *node2 = NULL;
	size_t *node3 = NULL;
	size_t *node4 = NULL;
	size_t *node5 = NULL;
	
	printf("pool suggested size: %ld\n", FSASuggestSize(8,10));
	
	fsa = FSAInit(ptr, FSASuggestSize(8,10), 8);
	
	printf("count free before alloc: %ld\n", FSACountFree(fsa));
	
	node1 = FSAAlloc(fsa);				/*fsa->start == 0*/
	printf("address: %p\n", (char *)node1);
	printf("setoff: %ld\n", *(size_t *)node1);
	node2 = FSAAlloc(fsa);				/*fsa->start == 8*/
	printf("address: %p\n", (char *)node2);
	printf("setoff: %ld\n", *(size_t *)node2);
	node3 = FSAAlloc(fsa);				/*fsa->start == 16*/
	printf("address: %p\n", (char *)node3);
	printf("setoff: %ld\n", *(size_t *)node3);
	node4 = FSAAlloc(fsa);				/*fsa->start == 24*/
	printf("address: %p\n", (char *)node4);
	printf("setoff: %ld\n", *(size_t *)node4);
	node5 = FSAAlloc(fsa);				/*fsa->start == 32*/
	printf("address: %p\n", (char *)node5);
	printf("setoff: %ld\n", *(size_t *)node5);
	
	printf("count free before free: %ld\n", FSACountFree(fsa));
	
	FSAFree(fsa,node1);
	FSAFree(fsa,node2);
	FSAFree(fsa,node3);
	FSAFree(fsa,node4);
	FSAFree(fsa,node5);
	
	printf("count free after free: %ld\n", FSACountFree(fsa));
	

	free(ptr);	
	
	return 0;
}
