/*********************************
 * Reviewer: Roy
 * Author: Shani Horden
 * File Name: fsa.c
 * ******************************/
#include <stdio.h> /*printf*/
#include <stdlib.h>/*malloc, free*/
#include <assert.h>/*assert*/

#include "fsa.h"

void TestSIzeFree();
void TestSuggest();
void TestAlloc();
void TestStruct();
void TestInitSmallSIze();


int main()
{
    TestSIzeFree();
    TestSuggest();
    TestAlloc();
    TestStruct();
    TestInitSmallSIze();

    /*void *ptr = NULL;
    void *ptr1 = NULL, *ptr2 = NULL;
    void *pool = malloc(99);
    fsa_t *fsa = FSAInit(pool, 99, 12);
    printf("%ld\n\n", FSASuggestSize(12, 10));

    printf("fsa- %p\n", fsa);
    ptr = FSAAlloc(fsa);
    printf("ptr- %p\n", ptr);

    printf ("%ld\n\n",(size_t)ptr-(size_t)fsa);

    free(pool);
    pool = malloc(40);
     printf("pool2- %p\n", pool);
    fsa = FSAInit(pool, 40, 12);

    printf ("size 2? %ld\n", FSACountFree(fsa));
    ptr = FSAAlloc(fsa);
    printf ("size 1? %ld\n", FSACountFree(fsa));
    ptr1 = FSAAlloc (fsa);
     printf ("size 0? %ld\n", FSACountFree(fsa));

    if (fsa)
    {
    printf("fsa2- %p\n", fsa);

    }


    if (ptr)
    {

    printf("ptr- %p\n", ptr);
    }

    if (ptr1)
    {

    printf("ptr1- %p\n", ptr1);
   
    }

    if (!ptr)
    {
        printf ("ptr null");
    }
    else
    {
         printf ("%ld\n",(size_t)ptr-(size_t)fsa);     
    }
   
     

     if (!ptr1)
    {
        printf ("ptr1 null");
    }
    else
    {

    printf ("%ld\n",(size_t)ptr1-(size_t)ptr);
    }


    ptr2 = FSAAlloc(fsa);

     if (!ptr2)
    {
        printf ("ptr2 null\n\n");
    }

    FSAFree(fsa, ptr);
    printf ("size 1? %ld\n", FSACountFree(fsa));
    FSAFree(fsa, ptr1);
    printf ("size 2? %ld\n", FSACountFree(fsa));


    free (pool);
*/


    return 0;
}


void TestSIzeFree()
{
    void *ptr = NULL;
    void *ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL, *ptr4 = NULL, *ptr5 = NULL;
    void *pool = malloc(55);
    fsa_t *fsa = FSAInit(pool, 55, 6);
	
    assert (5 == FSACountFree(fsa));

    ptr = FSAAlloc(fsa);
    assert (4 == FSACountFree(fsa));

    ptr1 = FSAAlloc(fsa);
    assert (3 == FSACountFree(fsa));

    ptr2 = FSAAlloc(fsa);
    assert (2 == FSACountFree(fsa));

    ptr3 = FSAAlloc(fsa);
    assert (1 == FSACountFree(fsa));

    ptr4 = FSAAlloc(fsa);
    assert (0 == FSACountFree(fsa));

    ptr5 = FSAAlloc(fsa);
    assert (0 == FSACountFree(fsa));

    FSAFree(fsa, ptr4);
    assert (1 == FSACountFree(fsa));

    FSAFree(fsa, ptr3);
    assert (2 == FSACountFree(fsa));

    FSAFree(fsa, ptr2);
    assert (3 == FSACountFree(fsa));

    FSAFree(fsa, ptr1);
    assert (4 == FSACountFree(fsa));

    FSAFree(fsa, ptr);
    assert (5 == FSACountFree(fsa));

    ptr5 = FSAAlloc(fsa);
    assert (4 == FSACountFree(fsa));

    FSAFree(fsa, ptr5);
    assert (5 == FSACountFree(fsa));

    free(pool);


}



void TestSuggest()
{
    assert (175 == FSASuggestSize(12, 10));	
    assert (3215 == FSASuggestSize(30, 100));
    assert (175 == FSASuggestSize(1, 20));
}



void TestAlloc()
{
    int arr[8]= {1,2,3,4,5,6,7,8};
    int *ptr = NULL;
    int *ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL, *ptr4 = NULL, *ptr5 = NULL;
    void *pool = malloc(FSASuggestSize(30,4));
    fsa_t *fsa = FSAInit(pool, FSASuggestSize(30,4), 30);

    ptr = (int*)FSAAlloc(fsa);
    assert(8 == (size_t)ptr - (size_t)fsa);
    *ptr = *arr;

    ptr1 = (int*)FSAAlloc(fsa);
    assert(32 == (size_t)ptr1 - (size_t)ptr);

    ptr2 = (int*)FSAAlloc(fsa);
    assert(32 == (size_t)ptr2 - (size_t)ptr1);

    ptr3 = (int*)FSAAlloc(fsa);
    assert(32 == (size_t)ptr3 - (size_t)ptr2);

    ptr4 = (int*)FSAAlloc(fsa);
    assert (NULL == ptr4);

    ptr5 = ptr3;

    FSAFree(fsa, ptr1);
    FSAFree(fsa, ptr3);
    ptr3 = NULL;
    assert(ptr3 != ptr5);

    ptr3 = (int*)FSAAlloc(fsa);
    assert(ptr3 == ptr5);

    assert (1 == *ptr);

    FSAFree(fsa,ptr);
    assert (1 != *ptr);

    free(pool);


}



void TestStruct()
{
    typedef struct pepole
    {
        char *name;
        size_t id;
    }pepole_t;


    pepole_t *p1 = NULL, *p2 = NULL, *p3 = NULL, *p4 = NULL;
    void *pool = malloc(FSASuggestSize(sizeof(pepole_t),4));
    fsa_t *fsa = FSAInit(pool, FSASuggestSize(sizeof(pepole_t),4),sizeof(pepole_t));

    p1 = (pepole_t*)FSAAlloc(fsa);
    p1->id = 2233;
    p1->name = "shani";


    p2 = (pepole_t*)FSAAlloc(fsa);
    p2->id = 8766;
    p2->name = "roy";

    printf("%ld\n", p1->id);
    printf("%s\n",p1->name);
    printf("%ld\n", p2->id);
    printf("%s\n",p2->name);

    FSAFree(fsa, p1);

    free (pool);

}


void TestInitSmallSIze()
{
    void *pool = malloc (10);
    fsa_t *fsa = FSAInit(pool, 10,8);

    free(pool);

   /* pool = malloc (24);
    fsa = FSAInit(pool, 24,12);
*/
}

