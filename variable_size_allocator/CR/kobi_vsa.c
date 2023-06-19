/*********************************
 * Reviewer: Viktor Fainshtein
 * Author: 
 * File Name: vsa.h
 * ******************************/

#include <assert.h> /* assert */
#include "vsa.h"

typedef struct vsa_meta_data meta_data_t;
#define ALIGN_TO sizeof(size_t)
#define META_DATA_SIZE sizeof(meta_data_t)
#define OFFSET ((size_t)(Align(sizeof(vsa_t), ALIGN_TO)))

static meta_data_t *Defragmentation(vsa_t *vsa, size_t aligned_variable_size);
static void *Align(size_t what, size_t to);

struct vsa
{
    size_t act_pool_size;
};

struct vsa_meta_data
{
    size_t meta_data;    
    #ifndef NDEBUG
    void *magic_num;
    #endif
};


vsa_t *VSAInit(void *pool, size_t pool_size)
{
    vsa_t *vsa = NULL;
    meta_data_t *start_point;							/*init start_point to NULL*/

    assert(pool);
    
    vsa = (vsa_t *)Align((size_t)pool, ALIGN_TO);
    start_point = (meta_data_t *)((char *)vsa + OFFSET);
    pool_size -= (char *)start_point - (char *)pool;
    pool_size = pool_size % ALIGN_TO ? ((size_t)Align(pool_size, ALIGN_TO) - ALIGN_TO) : pool_size;

    if((long)pool_size < (long)(META_DATA_SIZE + ALIGN_TO))
    {
        return NULL;
    }
    
    vsa->act_pool_size = pool_size;
    start_point->meta_data = pool_size - META_DATA_SIZE;
    #ifndef NDEBUG
    start_point->magic_num = (void *)-1;
    #endif

    return vsa;
}


void *VSAAlloc(vsa_t *vsa, size_t variable_size)
{
    size_t aligned_variable_size = (size_t)Align(variable_size, ALIGN_TO);	/*init vars to NULL before use and assert after the NULL and bofore the init*/
    size_t mask = 1;
    meta_data_t *curr = NULL;
    
    assert(vsa);

    curr = Defragmentation(vsa, aligned_variable_size);
    if(!curr)
    {
        return NULL;
    }

    if(curr->meta_data >= ALIGN_TO + aligned_variable_size + META_DATA_SIZE)
    {
        ((meta_data_t *)((char *)curr + META_DATA_SIZE + aligned_variable_size))->meta_data = curr->meta_data - META_DATA_SIZE - aligned_variable_size; 
        #ifndef NDEBUG
        ((meta_data_t *)((char *)curr + META_DATA_SIZE + aligned_variable_size))->magic_num = (void *)-1;
        #endif

        curr->meta_data = aligned_variable_size | mask;
        #ifndef NDEBUG
        curr->magic_num = (void *)-1;
        #endif
    }
    else
    {
        curr->meta_data = curr->meta_data | mask;
        #ifndef NDEBUG
        curr->magic_num = (void *)-1;
        #endif
    }
    
    return (void *)((char *)curr + META_DATA_SIZE) ;
}


size_t VSALargestChunckAvailable(vsa_t *vsa)					/*init vars to NULL before use and assert after the NULL and bofore the init*/
{
    size_t mask = 1;
    size_t largest = 0;
    meta_data_t *curr = (meta_data_t *)((char *)vsa + OFFSET);
    meta_data_t *next = (meta_data_t *)((char *)curr + META_DATA_SIZE + (curr->meta_data & ~mask));
    meta_data_t *end_of_pool = (meta_data_t *)((char *)curr + vsa->act_pool_size);
    meta_data_t *tmp = NULL;

    assert(vsa);

    tmp = Defragmentation(vsa, vsa->act_pool_size - META_DATA_SIZE);		/*de-frag can be a void function - less vars will be needed*/
    if(!tmp)
    {
        while(next < end_of_pool)
        {
            if(!(curr->meta_data & mask) && curr->meta_data > largest)
            {
                largest = curr->meta_data;
            }

            curr = next;
            next = (meta_data_t *)((char *)curr + META_DATA_SIZE + (curr->meta_data & ~mask));
        }
    
        return largest < curr->meta_data && !(curr->meta_data & mask) ? curr->meta_data : largest;
    }
  
    return tmp->meta_data;
}


void VSAFree(void *block)
{
    size_t mask = 1;

    assert(block);
    assert(((meta_data_t *)block - 1)->magic_num == (void *)-1);		
   
    ((meta_data_t *)block - 1)->meta_data = (((meta_data_t *)block - 1)->meta_data) & ~mask;   
}







static void *Align(size_t what, size_t to)
{
    return (void *)((what - 1 + to) & -to);
}

static meta_data_t *Defragmentation(vsa_t *vsa, size_t aligned_variable_size)	/*init vars to NULL before use and assert after the NULL and bofore the init*/
{
    size_t mask = 1;
    meta_data_t *curr = (meta_data_t *)((char *)vsa + OFFSET);			
    meta_data_t *next = (meta_data_t *)((char *)curr + META_DATA_SIZE + (curr->meta_data & ~mask));
    meta_data_t *end_of_pool = (meta_data_t *)((char *)curr + vsa->act_pool_size);

    while(next < end_of_pool && ((curr->meta_data & mask) || curr->meta_data < aligned_variable_size))
    {

        if(!(curr->meta_data & mask) && !(next->meta_data & mask))
        {
            curr->meta_data += next->meta_data + META_DATA_SIZE;
        }
        else
        {
            curr = next;
            next = (meta_data_t *)((char *)curr + META_DATA_SIZE + (curr->meta_data & ~mask));
        }
    }
    
    if(next == end_of_pool && ((curr->meta_data & mask) || curr->meta_data < aligned_variable_size))
    {
        return NULL;
    }

    return curr;    
}
