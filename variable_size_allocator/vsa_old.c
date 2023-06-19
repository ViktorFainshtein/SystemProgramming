#include <assert.h>	/*assert*/
#include "vsa.h"

struct vsa
{
	size_t list_size;
};

static void* align(size_t what, size_t to)
{
    return (void*)((what - 1 + to) & -to);
}

static void DeFrag(vsa_t *vsa)							
{
	size_t *free_list = NULL;
	size_t *temp = NULL;
	size_t * end_of_loop = NULL;
	size_t mask = (~(0) ^ 1);
	
	assert(vsa);
	
	free_list = (size_t *)((char *)vsa + (size_t)align(sizeof(vsa_t), sizeof(size_t)));
	end_of_loop = (size_t *)((char *)vsa + (size_t)align(sizeof(vsa_t), sizeof(size_t)) + vsa->list_size + sizeof(size_t));
	
	while((free_list < end_of_loop))
	{	
		temp = (size_t *)((char *)free_list + *free_list + sizeof(size_t));
		
		if((temp < end_of_loop) && (((*free_list & 1) == 0) && ((*temp & 1) == 0)))			
		{	
			*free_list = *free_list + *temp + sizeof(size_t);
			*temp = 0;
		}
		else
		{
			free_list = (size_t *)((char *)free_list + ((*free_list & mask) + sizeof(size_t)));
		}
	}
}

vsa_t *VSAInit(void *pool, size_t poolsize)
{
	vsa_t *vsa = NULL;
	size_t *free_list = NULL;
	size_t pool_carry = 0;
	size_t vsa_carry = 0;
	
	assert(pool);
	
	if(poolsize <= (size_t)align(sizeof(vsa_t),sizeof(size_t)) + sizeof(size_t) + sizeof(size_t))
	{
		return NULL;
	}
	
	vsa = align((size_t)pool, sizeof(size_t));				
	pool_carry = (size_t)vsa - (size_t)pool;			
	
	free_list = (size_t*)((char *)vsa + (size_t)align(sizeof(vsa_t), sizeof(size_t)));	
	vsa_carry = (size_t)free_list - (size_t)vsa;
	
	poolsize -= (vsa_carry + pool_carry + sizeof(size_t));			
	
	vsa->list_size = poolsize;	
	vsa->list_size &= ~(sizeof(size_t)-1);					
	*free_list = vsa->list_size;						
	
	return vsa;
}

void *VSAAlloc(vsa_t *vsa, size_t block_size)
{
	size_t *free_list = NULL;
	size_t old_size = 0;
	size_t mask = (~(0) ^ 1);
	size_t *end_of_pool = NULL;
	
	assert(vsa);

	block_size = (size_t)align(block_size, sizeof(size_t));			
	
	if(block_size > vsa->list_size)
	{
		return NULL;
	}

	free_list = (size_t *)((char *)vsa + (size_t)align(sizeof(vsa_t), sizeof(size_t)));
	end_of_pool = (size_t *)((char *)vsa + (size_t)align(sizeof(vsa_t), sizeof(size_t)) + vsa->list_size + sizeof(size_t));
	
	DeFrag(vsa);								
	
	while((end_of_pool > free_list) && ((*free_list & 1) || ((*free_list) < block_size)))	
	{
		free_list = (size_t *)((char *)free_list + ((*free_list & mask) + sizeof(size_t)));
	}
	
	if(free_list >= end_of_pool)
	{
		return NULL;
	}
	
	if(*free_list >= block_size + sizeof(size_t))
	{
		old_size = *free_list;
		*free_list = block_size ^ 1;						
		free_list = (size_t *)((char *)free_list + (sizeof(size_t) + block_size));				
		*free_list = old_size - block_size - sizeof(size_t);					
		free_list = (size_t *)((char *)free_list - block_size);
	}
	else
	{
		*free_list = block_size ^ 1;
		free_list = (size_t *)((char *)free_list + sizeof(size_t));
	}						

	return free_list;
}

void VSAFree(void *block)
{
	size_t *data = NULL;
	size_t mask = (~(0) ^ 1);
	
	assert(block);
	
	data = (size_t *)((char *)block - sizeof(size_t));
	*(size_t *)data = *(size_t *)data & mask;
}

size_t VSALargestChunckAvailable(vsa_t *vsa)
{
	size_t *free_list = NULL;
	size_t largest = 0;
	size_t mask = (~(0) ^ 1);
	size_t *end_of_loop = NULL;
	
	assert(vsa);
	
	free_list = (size_t *)((char *)vsa + (size_t)align(sizeof(vsa_t), sizeof(size_t)));

	if(*free_list == vsa->list_size)
	{
		return vsa->list_size;
	}

	DeFrag(vsa);
	
	end_of_loop = (size_t *)((char *)vsa + (size_t)align(sizeof(vsa_t), sizeof(size_t)) + vsa->list_size + sizeof(size_t));
	
	while(free_list < end_of_loop)
	{		
		if(((*free_list & 1) == 0) && (*free_list > largest))
		{
			largest = *free_list;
		}	

		free_list = (size_t *)((char *)free_list + ((*free_list & mask) + sizeof(size_t)));
	}
	
	return largest;
}














