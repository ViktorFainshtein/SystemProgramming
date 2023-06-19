/*********************************
 * Reviewer: Shani Horden
 * Author: Viktor Fainshtein
 * File Name: fsa.c
 * ******************************/
#include <assert.h>	/*assert*/
#include "fsa.h"

struct fsa{
	size_t start;
};

static void* align(size_t what, size_t to)
{
    return (void*)((what - 1 + to) & -to);
}

fsa_t *FSAInit(void *pool, size_t pool_size, size_t block_size)
{
	fsa_t *fsa = NULL;
	char *free_list = NULL;
	size_t lost_data = 0;
	size_t lost_struct_data = 0;
	size_t i = 0;

	assert(pool);
	
	fsa = align((size_t)pool, sizeof(size_t));
	
	if(pool_size < (size_t)align(block_size, sizeof(size_t)) + (size_t)align(sizeof(fsa_t), sizeof(size_t)) + sizeof(size_t)-1) 
	{
		return NULL;
	}
	
	free_list = (char *)fsa + (size_t)align(sizeof(fsa_t), sizeof(size_t));
	fsa->start = 0;
	block_size = (size_t)align(block_size, sizeof(size_t));			
	
	lost_data = (char *)fsa - (char *)pool;
	lost_struct_data = (char *)free_list - (char *)fsa;
	
	while(i < (pool_size - lost_data - block_size - lost_struct_data))			
	{
		*(size_t *)((char *)free_list + i) = block_size + i;
		i += block_size;
	}
	
	*(int *)(free_list + i) = -1;

	return fsa;
}

void *FSAAlloc(fsa_t *fsa)
{
	void *free_list = NULL;
	
	assert(fsa);
	if(*(int *)((char *)fsa + (size_t)align(sizeof(fsa_t), sizeof(size_t)) + fsa->start) == -1)
	{
		return NULL;
	}
	
	free_list = (char *)fsa + (size_t)align(sizeof(fsa_t), sizeof(size_t)) + fsa->start;
	fsa->start = *(size_t *)((char *)fsa + (size_t)align(sizeof(fsa_t), sizeof(size_t)) + fsa->start);
	
	return free_list;
}

void FSAFree(fsa_t *fsa, void *block_address)
{
	assert(fsa);
	assert(block_address);
	
	*(size_t *)block_address = fsa->start; 
	fsa->start = (size_t)block_address - (size_t)fsa - (size_t)align(sizeof(fsa_t), sizeof(size_t));
}

size_t FSACountFree(fsa_t *fsa)
{
	size_t count = 0;
	char *ptr = NULL;
	char *free_list = NULL;
	
	assert(fsa);
	
	free_list = (char *)fsa + (size_t)align(sizeof(fsa_t), sizeof(size_t));
	
	ptr = (char *)free_list + fsa->start;
	
	while(*(int *)(ptr) != -1)
	{
		ptr = (char *)free_list + *(size_t *)ptr;
		count++;
	}
	
	return count;
}

size_t FSASuggestSize(size_t block_size, size_t num_of_blocks)
{
	size_t res = 0;
	size_t struct_size = 0;
	
	block_size = (size_t)align(block_size, sizeof(size_t));
	struct_size = (size_t)align(sizeof(fsa_t), sizeof(size_t));
	
	res = (block_size * num_of_blocks) + struct_size + sizeof(size_t)-1;
	
	return res;
}



















