#include <stdio.h>	/*printf*/
#include <stdlib.h>	/*malloc free*/
#include <assert.h>
#include "vsa.h"











int main()
{
	void *pool = malloc(100);
	
	char *node1 = NULL;
	char *node2 = NULL;
	char *node3 = NULL;
	char *node4 = NULL;
	
	vsa_t *vsa = VSAInit(pool, 100);	
	if(vsa == NULL)
	{
		printf("Failed To Initialize\n");
	}
	
	printf("LARGEST CHUNK: %ld\n\n", VSALargestChunckAvailable(vsa));
	
	if(VSALargestChunckAvailable(vsa) != 72)
	{
		printf("Incorrect Buffer Size After Initialization\n");
	}
	
	node1 = VSAAlloc(vsa, 8);	/*72 - 8 - 16 = 72 - 24 = 48*/
	if(node1 == NULL)
	{
		printf("Failed To Allocate First Block(node1)\n");
	}				
	
	printf("LARGEST CHUNK: %ld\n\n", VSALargestChunckAvailable(vsa));
	node2 = VSAAlloc(vsa, 16);	/*48 - 16 - 16 = 48 -32 = 16*/
	if(node2 == NULL)
	{
		printf("Failed To Allocate Second Block(node2)\n");
	}						
	
	printf("LARGEST CHUNK: %ld\n\n", VSALargestChunckAvailable(vsa));
	node3 = VSAAlloc(vsa, 8);	/* 16 - 8 - (?) = 0*/
	if(node3 == NULL)
	{
		printf("Failed To Allocate Third Block(node3)\n");
	}						
	
	printf("LARGEST CHUNK: %ld\n\n", VSALargestChunckAvailable(vsa));
	
	node4 = VSAAlloc(vsa, 50);	/*NULL*/
	if(node4 != NULL)
	{
		printf("Allocated A Block That Should Not Be Allocated(node4)\n");
	}
	printf("ALLOC node4:\naddress: %p\n",node4);

	if(VSALargestChunckAvailable(vsa) != 0)
	{
		printf("Incorrect Largest Chunck Size(after 3 alloc's)\n");
	}
	
	printf("LARGEST CHUNK: %ld\n\n", VSALargestChunckAvailable(vsa));
	
	VSAFree(node1);
	
	VSAFree(node2);
	
	VSAFree(node3);
	

	if(VSALargestChunckAvailable(vsa) != 72)
	{
		printf("Incorrect Largest Chunck Size(after free all)\n");
	}
		
	printf("LARGEST CHUNK: %ld\n\n", VSALargestChunckAvailable(vsa));
	
	node1 = VSAAlloc(vsa, 150);
	printf("ALLOC node1:\naddress: %p\n",node1);
	
	if(node1 != NULL)
	{
		printf("Allocated A Block With Bigger Size Then Buffer");
	}
	
	node1 = VSAAlloc(vsa, 50);	/*72 - 56 - 16 = 0*/
	
	if(node1 == NULL)
	{
		printf("Failed To Allocate A Legit Block After Defrag(node1 - size 50)\n");
	}

	if(VSALargestChunckAvailable(vsa) != 0)
	{
		printf("Failed To DeFrag Correctly After Allocation\n");
	}
	
	printf("LARGEST CHUNK: %ld\n\n", VSALargestChunckAvailable(vsa));
	
	VSAFree(node1);

	free(pool);
	
	return 0;
}



