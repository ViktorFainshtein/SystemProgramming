#include <assert.h>
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc-free */
#include "vsa.h"

static int Test1FuncRelease();
static int TestFregmentationRelease();

static int Test1FuncDebug();
static int TestFregmentationDebug();

int main(void)
{
	
	#ifndef NDEBUG
		printf("--------------Debug mode--------------\n\n");
		Test1FuncDebug();
		TestFregmentationDebug();
	#else
		printf("-------------Release mode------------\n\n");
   		Test1FuncRelease();
		TestFregmentationRelease();
	#endif

	return (0);
}

/* only in debug mode */
static int Test1FuncDebug()
{
	size_t pool_size = 100;
	void *pool = NULL;
	vsa_t *vsa = NULL;
	
	void *alloc1 = NULL;
	void *alloc2 = NULL;
	void *alloc3 = NULL;
	
	pool = malloc(pool_size);
	vsa = VSAInit(pool, pool_size);
	
	if(VSALargestChunckAvailable(vsa) != 72)
	{
		printf("VSALargestChunckAvailable(vsa): %lu\n", VSALargestChunckAvailable(vsa));
		printf("VSALargestChunckAvailable failed!\n");
		/*return (1);*/
	}
	
	alloc1 = VSAAlloc(vsa, 10);
	assert(alloc1);
	
	if(VSALargestChunckAvailable(vsa) != 40)
	{
		printf("line 54, Alloc1 failed!\n\n");
		return (1);
	}
	
	alloc2 = VSAAlloc(vsa, 40);
	assert(alloc2);
	
	if(VSALargestChunckAvailable(vsa) != 0) /* only 4 left but not enough for metadata, 
						so its allocation of 60 */
	{
		printf("%lu\n", VSALargestChunckAvailable(vsa));
		printf("Alloc2 failed!\n\n");
		return (1);
	}
	
	alloc3 = VSAAlloc(vsa, 4); /* should fail */
	if(alloc3 != NULL)
	{
		printf("alloc3 failed");
		return (1);
	}
	
	
	VSAFree(alloc2);
	
	if(VSALargestChunckAvailable(vsa) != 40)
	{
		printf("\n");
		printf("Free2 failed! recived: %lu", VSALargestChunckAvailable(vsa));
		return (1);
	}
	
	alloc2 = VSAAlloc(vsa, 40);
	VSAFree(alloc1);
	
	if(VSALargestChunckAvailable(vsa) != 16)
	{
		printf("\n");
		printf("Free1 failed! recived: %lu", VSALargestChunckAvailable(vsa));
		return (1);
	}
	
	free(pool);
	
	return(0);
}

static int TestFregmentationDebug()
{
	size_t pool_size = 200;
	void *pool = NULL;
	vsa_t *vsa = NULL;
	
	void *alloc1 = NULL;
	void *alloc2 = NULL;
	void *alloc3 = NULL;
	void *alloc4 = NULL;
	
	pool = malloc(pool_size);
	vsa = VSAInit(pool, pool_size);
	
	alloc1 = VSAAlloc(vsa, 40);
	assert(alloc1);
	
	assert(VSALargestChunckAvailable(vsa) == 120);
	
	alloc2 = VSAAlloc(vsa, 40);
	assert(alloc2);

	assert(VSALargestChunckAvailable(vsa) == 64);
	
	alloc3 = VSAAlloc(vsa, 40);
	assert(alloc3);

	assert(VSALargestChunckAvailable(vsa) == 8);

	alloc4 = VSAAlloc(vsa, 8);
	assert(alloc4);
	
	assert(VSALargestChunckAvailable(vsa) == 0);
	
	VSAFree(alloc4);
	
	assert(VSALargestChunckAvailable(vsa) == 8); 
	
	VSAFree(alloc3);
	
	assert(VSALargestChunckAvailable(vsa) == 64); 
	
	/*printf("VSALargestChunckAvailable(vsa): %lu\n", VSALargestChunckAvailable(vsa));*/
	
	
	alloc3 = VSAAlloc(vsa, 80); /* should fail */
	
	
	assert(VSALargestChunckAvailable(vsa) == 64); 
	
	VSAFree(alloc2);
	
	alloc3 = VSAAlloc(vsa, 80);
	assert(alloc3);
	
	VSAFree(alloc1); /* now only 1, and 4 are free, shouldnt work to alloc */
	
	free(pool);
	
	return(0);
}




/* only in release mode */
static int Test1FuncRelease()
{
	size_t pool_size = 100;
	void *pool = NULL;
	vsa_t *vsa = NULL;
	
	void *alloc1 = NULL;
	void *alloc2 = NULL;
	void *alloc3 = NULL;
	
	pool = malloc(pool_size);
	vsa = VSAInit(pool, pool_size);
	
	if(VSALargestChunckAvailable(vsa) != 80)
	{
		printf("VSALargestChunckAvailable failed!");
		return (1);
	}
	
	alloc1 = VSAAlloc(vsa, 20);
	assert(alloc1);
	
	if(VSALargestChunckAvailable(vsa) != 48)
	{
		printf("%lu\n", VSALargestChunckAvailable(vsa));
		printf("Alloc1 failed!");
		return (1);
	}
	
	alloc2 = VSAAlloc(vsa, 40);
	assert(alloc2);
	
	if(VSALargestChunckAvailable(vsa) != 0)
	{
		printf("%lu\n", VSALargestChunckAvailable(vsa));
		printf("Alloc2 failed!");
		return (1);
	}
	
	alloc3 = VSAAlloc(vsa, 4); /* should fail */
	if(alloc3 != NULL)
	{
		printf("alloc3 failed");
		return (1);
	}
	
	
	VSAFree(alloc2);
	
	if(VSALargestChunckAvailable(vsa) != 48)
	{
		printf("\n");
		printf("Free2 failed! recived: %lu", VSALargestChunckAvailable(vsa));
		return (1);
	}
	
	alloc2 = VSAAlloc(vsa, 40);
	VSAFree(alloc1);
	
	if(VSALargestChunckAvailable(vsa) != 24)
	{
		printf("\n");
		printf("Free1 failed! recived: %lu", VSALargestChunckAvailable(vsa));
		return (1);
	}
	
	
	return(0);
}

static int TestFregmentationRelease()
{
	size_t pool_size = 200;
	void *pool = NULL;
	vsa_t *vsa = NULL;
	
	void *alloc1 = NULL;
	void *alloc2 = NULL;
	void *alloc3 = NULL;
	void *alloc4 = NULL;
	
	pool = malloc(pool_size);
	vsa = VSAInit(pool, pool_size);
	
	alloc1 = VSAAlloc(vsa, 40);
	assert(alloc1);
	
	assert(VSALargestChunckAvailable(vsa) == 144);
	
	alloc2 = VSAAlloc(vsa, 40);
	assert(alloc2);

	assert(VSALargestChunckAvailable(vsa) == 96);
	
	alloc3 = VSAAlloc(vsa, 40);
	assert(alloc3);

	assert(VSALargestChunckAvailable(vsa) == 48);

	alloc4 = VSAAlloc(vsa, 40);
	assert(alloc4);
	
	assert(VSALargestChunckAvailable(vsa) == 0);
	
	VSAFree(alloc3);
	VSAFree(alloc4);
	
	assert(VSALargestChunckAvailable(vsa) == 48); 
	
	
	alloc3 = VSAAlloc(vsa, 80);
	assert(alloc3);

	
	VSAFree(alloc1); /* now only 1, and 4 are free, shouldnt work to alloc */
	
	return(0);
}
