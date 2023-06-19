/*********************************
 * Reviewer: 
 * Author: Sigal
 * File Name: vsa_test.c
 * ******************************/
#include <stdlib.h>/*malloc*/
#include <stdio.h>
#include <assert.h>
#include "vsa.h"

void TestAlloc();
void TestFree();
void TestDefragmentation();

int main()
{

    TestAlloc();
    TestFree();
    TestDefragmentation();

    
    return 0;
}

void TestAlloc()
{
    void *pool = malloc(32);
    vsa_t *vsa= VSAInit(pool,32);
    void *ptr = NULL, *ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL;
    
  
    assert(vsa);
    printf("************\n");
    ptr = VSAAlloc(vsa, 8);
    assert(ptr);

    ptr1 = VSAAlloc(vsa, 6);
    assert(NULL == ptr1);
    printf("222222222\n");

    ptr2 = VSAAlloc(vsa, 6);
    assert(NULL == ptr2);
    free(pool);


    pool = malloc(100);

    printf("************\n");
    
    vsa= VSAInit(pool,100);

    ptr = VSAAlloc(vsa, 8);
    assert(ptr);

    ptr1 = VSAAlloc(vsa, 12);
    assert(ptr1);

    ptr2 = VSAAlloc(vsa, 20);
    assert(!ptr2);

    ptr3 = VSAAlloc(vsa, 15);
    assert(ptr3);

    free(pool);
}

void TestFree()
{
    void *pool = malloc(80);
    vsa_t *vsa= VSAInit(pool,80);
    void *ptr = NULL, *ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL;

    ptr=VSAAlloc(vsa,sizeof(int));
    assert(ptr);
    ptr1=VSAAlloc(vsa,sizeof(int));
    assert(ptr1);
    ptr2=VSAAlloc(vsa,sizeof(int));
    assert(ptr2);

    printf("ptr: %p, ptr1: %p, ptr2: %p\n", ptr, ptr1, ptr2);

    VSAFree(ptr2);

    printf("large:%lu\n",VSALargestChunckAvailable(vsa));
    assert(8==VSALargestChunckAvailable(vsa));

    ptr3=VSAAlloc(vsa,sizeof(int));
    assert(ptr3);
    assert(0==VSALargestChunckAvailable(vsa));

    free(pool);
}

void TestDefragmentation()
{
    void *pool = malloc(200);
    vsa_t *vsa= VSAInit(pool,200);
    void *ptr = NULL, *ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL, *ptr4 = NULL; 

    ptr=VSAAlloc(vsa,80);
    assert(ptr);
    ptr1=VSAAlloc(vsa,80);
    assert(ptr1);

    VSAFree(ptr);

    ptr4=VSAAlloc(vsa,12);
    assert(ptr);
    ptr3=VSAAlloc(vsa,12);
    assert(ptr1);
    ptr2=VSAAlloc(vsa,12);
    assert(ptr2);

    VSAFree(ptr4);
    VSAFree(ptr3);
    VSAFree(ptr2);

    printf("ptr: %p, ptr1: %p, ptr2: %p\n", ptr, ptr1, ptr2);

    /*Defragmentaion(vsa, 80);*/

    printf("def:%lu\n",VSALargestChunckAvailable(vsa));

    assert(80==VSALargestChunckAvailable(vsa));

    free(pool);
}