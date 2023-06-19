/*********************************
 * Reviewer: 
 * Author: yisrael fisch
 * File Name: fsa.h
 * ******************************/

#include <assert.h>/*assert*/
#include "fsa.h"

#define WORD_SIZE (sizeof(size_t))
/*macro for the ofsset of the start of the pool of blocks from fsa */
#define BLOCK_START (size_t)Align(sizeof(fsa_t), WORD_SIZE)

struct fsa
{
    size_t offset_from_fsa;   
};

static void* Align(size_t what, size_t to)
{
    return (void*)((what - 1 + to) & -to);
}



size_t FSASuggestSize(size_t block_size, size_t num_of_blocks)
{
    size_t actual_block_size = (size_t)Align(block_size, WORD_SIZE);
    
    return ((WORD_SIZE-1) + (size_t)Align(sizeof(fsa_t), WORD_SIZE) + 
            (actual_block_size * num_of_blocks));
}

fsa_t *FSAInit(void *pool, size_t pool_size, size_t block_size)
{
    fsa_t *fsa = NULL;
    char *block = NULL; 
    size_t i = 0;
    assert(pool);

    fsa = Align((size_t)pool, WORD_SIZE);
    block_size = (size_t)Align(block_size, WORD_SIZE);

    block = (char *)fsa + (size_t)Align(sizeof(fsa_t), WORD_SIZE);
    fsa->offset_from_fsa = 0;

    pool_size -= (char *)block - (char *)pool;    
    while(i <= pool_size - block_size * 2)
    {
        i += block_size;
        *(size_t *)block = i;
        block += block_size;
    }

    *(int *)block = -1;

    return fsa;
}

void *FSAAlloc(fsa_t *fsa)
{
    void *temp = NULL;
    assert(fsa);
    if(-1 == (int)fsa->offset_from_fsa)
    {
        return NULL;
    }

    temp = (char *)fsa + BLOCK_START + fsa->offset_from_fsa ; 
    fsa->offset_from_fsa = *(size_t *)((char*)fsa + BLOCK_START + fsa->offset_from_fsa);
    return temp;
}

void FSAFree(fsa_t *fsa, void *block_address)
{
    assert(fsa);
    assert(block_address);

    *(size_t *)block_address = fsa->offset_from_fsa;
    fsa->offset_from_fsa = (size_t)((char *)block_address - ((char *)fsa + BLOCK_START));
}

size_t FSACountFree(fsa_t *fsa)
{
    size_t counter = 0;
    size_t temp_offset = 0; 
    
    assert(fsa);
    temp_offset = fsa->offset_from_fsa;

    while(-1 != (int)temp_offset)
    {
        ++counter;
        temp_offset = *(size_t *)(((char*)fsa) + BLOCK_START + temp_offset);

    }

    return counter;
}



