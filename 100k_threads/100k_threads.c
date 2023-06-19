#include <stdio.h>      /*io, printf*/
#include <stdlib.h>     /*malloc free*/
#include <unistd.h>     /*sleep*/
#include <pthread.h>    /*pthread_create*/

#include <ulimit.h>         /*ulimit*/
#include <sys/time.h>       /*setrlimit*/
#include <sys/resource.h>   /*setrlimit*/


/*32754 - without detach*/
#define SIZE 16


#define NUMBER 8589869056

void *WriteToArr(void *i);
void PrlongArr();
void CheckIndex();

void *SumOfDivisors(void *data)
{
    long sum_of_divisors = 0;
    long i = 0;

    for(i = (long)data; (i < (long)data + (NUMBER/SIZE)) && (i < NUMBER); ++i)
    {
        if(0 == NUMBER % i)
        {
            sum_of_divisors += i;
        }
    }
    printf("sum: %ld\n", sum_of_divisors);
    return (void *)sum_of_divisors;
}

/* long SumOfDivisors2(long number)
{
    long sum = 0;
    long i = 0;
    for(i = 1; i<number; ++i)
    {
        if(0 == number % i)
        {
            sum += i;
        }
    }
    return sum;
} */

int main()
{
    long i = 0;
    long err_no = 0;
    pthread_t thread[SIZE];
    long arr[SIZE] = {0};
    void *res[SIZE];

    long sum = 0;
    long index = 1;

/*     getrlimit(RLIMIT_NPROC, &limit);

    printf("min limit: %ld\n", limit.rlim_cur);
    printf("max limit: %ld\n", limit.rlim_max); */

    for(i = 0; i < SIZE; i++)
    {
        err_no = pthread_create(thread+i, NULL, SumOfDivisors, (void *)index);
        index += (NUMBER/SIZE);
/*         err_no = pthread_create(thread+i, NULL, WriteToArr, (void *)i);
        if(err_no != 0)
        {
            while (err_no != 0)
            {
                err_no = pthread_create(thread+i, NULL, WriteToArr, (void *)i);
            }
        } */
    }
    /* sleep(1); */
    for(i = 0; i<SIZE; i++)
    {
        pthread_join(thread[i], res+i);
    }
/*     printf("\n%ld\n", SumOfDivisors2(NUMBER)); */
    for(i = 0; i<SIZE; i++)
    {
        sum += (long)res[i];
        printf("res[i]: %ld\t", (long)res[i]);
    }
    printf("\n%ld\n", sum);
/*     PrlongArr();

    CheckIndex();
 */

    return 0;
}

/* void *WriteToArr(void *i)
{
    long j = (long)i;
    arr[j] = j;
     pthread_detach(pthread_self());
    return NULL;
} */

/* void PrlongArr()
{
    long i = 0;
    for(i = 0; i < SIZE; i++)
    {
        printf("arr[%ld] = %ld\n", i, arr[i]);
    }
} */

/* void CheckIndex()
{
    long i = 0;
    for(i=0;i<SIZE; i++)
    {
        if(arr[i]!= i)
        {
            printf("incorrect!!!\n");
        }
    }
} */