/*********************************
 * Reviewer: Viktor Fainshtein
 * Author: Sigal
 * File Name: vsa.c
 * ******************************/
#include <assert.h>	/*assert*/
#include "vsa.h"

struct vsa
{
	size_t list_size;
};

typedef struct vsa_md
{
    size_t block_size;    
    #ifndef NDEBUG
    void *magic_number;
    #endif
}vsa_md_t;

static void* align(size_t what, size_t to)
{
    return (void*)((what - 1 + to) & -to);
}

static void DeFrag(vsa_t *vsa)							
{
	char *vmd = NULL;
	char *next = NULL;
	char *end_of_loop = NULL;
	size_t set_off = (~(0) ^ 1);
	
	size_t step = 0;
	
	assert(vsa);
	
	vmd = (char *)vsa + (size_t)align(sizeof(vsa_t), sizeof(size_t));
	end_of_loop = (char *)vsa + (size_t)align(sizeof(vsa_t), sizeof(size_t)) + ((vsa_t *)vsa)->list_size + (size_t)align(sizeof(vsa_md_t), sizeof(size_t));
	
	next = vmd;
	
	while(next < end_of_loop)
	{	
		step += ((((vsa_md_t *)vmd)->block_size) & set_off);

		if(step >= vsa->list_size)
		{
			break;
		}
		
		next = (char *)vmd + (((((vsa_md_t *)vmd)->block_size) & set_off) + sizeof(vsa_md_t));
		
		if(next >= end_of_loop)
		{
			break;
		}
		
		if(((size_t)next < (size_t)end_of_loop) && ((((((vsa_md_t *)vmd)->block_size) & 1) == 0) && (((((vsa_md_t *)next)->block_size) & 1) == 0)))				
		{	
			((vsa_md_t *)vmd)->block_size = ((vsa_md_t *)vmd)->block_size + ((vsa_md_t *)next)->block_size + sizeof(vsa_md_t);
		}
		else
		{
			vmd = next;
		}
	}
}

vsa_t *VSAInit(void *pool, size_t poolsize)
{
	vsa_t *vsa = NULL;
	vsa_md_t *vmd = NULL;
	size_t pool_carry = 0;
	
	assert(pool);
	
	vsa = align((size_t)pool, sizeof(size_t));	
	vmd = (vsa_md_t *)((char *)vsa + (size_t)align(sizeof(vsa_t),sizeof(size_t)));			
	
	pool_carry = (size_t)vsa - (size_t)pool;				
	
	if(poolsize < ((size_t)align(sizeof(vsa_t),sizeof(size_t)) + (size_t)align(sizeof(vsa_md_t),sizeof(size_t)) + sizeof(size_t) + pool_carry))
	{
		return NULL;
	}

	poolsize = poolsize - ((size_t)align(sizeof(vsa_t),sizeof(size_t)) + pool_carry + (size_t)align(sizeof(vsa_md_t),sizeof(size_t)));			
	
	poolsize &= ~(sizeof(size_t)-1);
	
	vsa->list_size = poolsize;						
	vmd->block_size = poolsize;	
	
	#ifndef NDEBUG
	vmd->magic_number = (void *)0xABADCAFE;
	#endif					
	
	return vsa;
}

void *VSAAlloc(vsa_t *vsa, size_t block_size)
{
	vsa_md_t *vmd = NULL;
	vsa_md_t *end_of_pool = NULL;
	size_t old_size = 0;
	size_t set_off = (~(0) ^ 1);
	
	size_t step = 0;
	
	assert(vsa);

	block_size = (size_t)align(block_size, sizeof(size_t));			
	
	if(block_size > vsa->list_size)
	{
		return NULL;
	}

	vmd = (vsa_md_t *)((char *)vsa + (size_t)align(sizeof(vsa_t), sizeof(size_t)));
	end_of_pool = (vsa_md_t *)((char *)vsa + (size_t)align(sizeof(vsa_t), sizeof(size_t)) + vsa->list_size + sizeof(vsa_md_t));
	
	DeFrag(vsa);								
	
	while(end_of_pool > vmd)	
	{
		step += ((vmd->block_size) & set_off);
		if(!(vmd->block_size & 1) && (vmd->block_size >= block_size))
		{
			break;
		}
		if(step + block_size > vsa->list_size)
		{
			return NULL;
		}
		vmd = (vsa_md_t *)((char *)vmd + ((vmd->block_size & set_off) + (size_t)align(sizeof(vsa_md_t), sizeof(size_t))));
	}
		
	if(end_of_pool <= vmd)
	{
		return NULL;
	}
	
	if(vmd->block_size >= block_size + sizeof(vsa_md_t))							
	{
		old_size = vmd->block_size;
		vmd->block_size = block_size ^ 1;						
		vmd = (vsa_md_t *)((char *)vmd + ((size_t)align(sizeof(vsa_md_t), sizeof(size_t)) + block_size));				
		vmd->block_size = old_size - block_size - (size_t)align(sizeof(vsa_md_t), sizeof(size_t));	
		#ifndef NDEBUG
		vmd->magic_number = (void *)0xABADCAFE;
		#endif				
		vmd = (vsa_md_t *)((char *)vmd - block_size);
	}
	else
	{
		((vsa_md_t *)((char *)vmd))->block_size ^= 1;
		vmd = (vsa_md_t *)((char *)vmd + (size_t)align(sizeof(vsa_md_t), sizeof(size_t)));
	}						

	return vmd;
}

void VSAFree(void *block)
{
	vsa_md_t *vmd = NULL;
	size_t set_off = (~(0) ^ 1);
	
	assert(block);
	
	vmd = (vsa_md_t *)((char *)block - sizeof(vsa_md_t));
	
	assert(vmd->magic_number == (void *)0xABADCAFE);		
   
	vmd->block_size = vmd->block_size & set_off;
}

size_t VSALargestChunckAvailable(vsa_t *vsa)
{
	vsa_md_t *vmd = NULL;
	vsa_md_t *end_of_loop = NULL;
	size_t largest = 0;
	size_t set_off = (~(0) ^ 1);
	
	assert(vsa);
	
	vmd = (vsa_md_t *)((char *)vsa + (size_t)align(sizeof(vsa_t), sizeof(size_t)));

	if(vmd->block_size == vsa->list_size)
	{
		return vsa->list_size;
	}

	DeFrag(vsa);
	
	end_of_loop = (vsa_md_t *)((char *)vsa + (size_t)align(sizeof(vsa_t), sizeof(size_t)) + sizeof(vsa_md_t) + vsa->list_size);
	
	while(vmd < end_of_loop)
	{		
		if((((vmd->block_size) & 1) == 0) && ((vmd->block_size) > largest))
		{
			largest = vmd->block_size;
		}	

		vmd = (vsa_md_t *)((char *)vmd + (((vmd->block_size) & set_off) + sizeof(vsa_md_t)));
	}
	
	return largest;
}














