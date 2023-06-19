#include <stdio.h>  /*printf*/
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h> /*strcmp*/
#include <stdlib.h> /*rand*/
#include "dll.h"    /*DListPushFront DListPopBack DListCreate DListDestroy*/
#include "circular_buffer.h"    /*CircBuffRead CircBuffWrite CircBuffCreate CircBuffDestroy*/

/*------------------------- Circular Fixed Buffer : -------------------------*/
#define BUFFSIZE 10

int cf_buff[BUFFSIZE] = {0};
int cfb_read = 0;
int cfb_write = 0;

int CircularFixedBufferRead(int cf_buff[])
{
    int val = 0;
    val = cf_buff[cfb_read];
    ++cfb_read;
    cfb_read %= BUFFSIZE; 
    return val;
}

void CircularFixedBufferWrite(int cf_buff[], int data)
{
    cf_buff[cfb_write] = data;
    ++cfb_write;
    cfb_write %= BUFFSIZE;
}

/*------------------------------- EXERCISE 1: -------------------------------*/
#if 0 
pthread_t producer;
pthread_t consumer;
volatile int isDataReady = 1;

void *Produce(void *msg)
{
    while(1)
    {
        while(__atomic_load_n(&isDataReady, __ATOMIC_SEQ_CST), 0 == isDataReady);
        fgets((char *)msg, 9, stdin);
        *((char *)msg+strlen((char *)msg)-1) = '\0';
        __atomic_store_n(&isDataReady, 0, __ATOMIC_SEQ_CST);
        if(strcmp((char *)msg, "exit") == 0)
        {
            break;
        }
    }

    return NULL;
}

void *Consume(void *msg)
{
    while(1)
    {
        while(__atomic_load_n(&isDataReady, __ATOMIC_SEQ_CST), 1 == isDataReady);
        printf("consumer prints: %s\n", (char *)msg);
        __atomic_store_n(&isDataReady, 1, __ATOMIC_SEQ_CST);
        if(strcmp((char *)msg, "exit") == 0)
        {
            break;
        }
    }
    return NULL;
}

int main()
{
    int err_no = 0;
    char msg[10] = {'0'};
    
    err_no = pthread_create(&producer, NULL, Produce, msg);
    err_no = pthread_create(&consumer, NULL, Consume, msg);

    pthread_join(producer, NULL);

    return 0;
}
#endif
/*------------------------------- EXERCISE 2: -------------------------------*/
#if 0
#define SIZE 5
pthread_mutex_t lock;
int res_g = 0;
int poison_apple_g = 770;
int shut_down_g = 0;
int pro_count = 0;
int con_count = 0;


void *Produce(void *dll)
{
    int data = 0;
    while(1)
    {
        sleep(1);
        pthread_mutex_lock(&lock);
        ++pro_count;
        if(1 == shut_down_g)
        {  
            pthread_mutex_unlock(&lock);
            break;

        }
        else
        {
            data = rand()%100;
            DListPushFront((dll_t *)dll, &data);
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

void *Consume(void *dll)
{
    int res = 0;
    while(1)
    {
        while(DListIsEmpty((dll_t *)dll));
        pthread_mutex_lock(&lock);
        if(!DListIsEmpty((dll_t *)dll))
        {
            ++con_count;
            res = *(int *)DListPopBack((dll_t *)dll);
            if(res == poison_apple_g)
            {
                pthread_mutex_unlock(&lock);
                break;
            }
            res_g += res;
            printf("current res: %d\n", res);
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main()
{
    pthread_t producer[SIZE];
    pthread_t consumer[SIZE];

    int err_no = 0;
    int i = 0;

    dll_t *dll = NULL;
    dll = DListCreate();

    pthread_mutex_init(&lock, NULL);

    for(i=0; i < SIZE; i++)
    {
        err_no = pthread_create(producer+i, NULL, Produce, (void *)dll);
        err_no = pthread_create(consumer+i, NULL, Consume, (void *)dll);
    }
    
    sleep(3);
    shut_down_g = 1;

    for(i = 0; i < SIZE; i++)
    {
        pthread_join(producer[i], NULL);
    }

    pthread_mutex_lock(&lock);
    for(i=0; i<SIZE; i++)
    {
        DListPushFront(dll, &poison_apple_g);
    }
    pthread_mutex_unlock(&lock);

    for(i = 0; i < SIZE; i++)
    {
        pthread_join(consumer[i], NULL);
    }

 
    printf("total res: %d\n",res_g);
    printf("producer counter: %d\n",pro_count);
    printf("consumer counter: %d\n",con_count);

    pthread_mutex_destroy(&lock);
    DListDestroy(dll);

    return 0;
}
#endif
/*------------------------------- EXERCISE 3: -------------------------------*/
#if 0 
#define SIZE 5

pthread_mutex_t lock;
sem_t semaphore;

int res_g = 0;
int poison_apple_g = 770;
int shut_down_g = 0;
int pro_count = 0;
int con_count = 0;

void *Produce(void *dll)
{
    int data = 0;
    while(1)
    {
        sleep(1);
        pthread_mutex_lock(&lock);
        ++pro_count;
        if(1 == shut_down_g)
        {  
            pthread_mutex_unlock(&lock);
            break;

        }
        else
        {
            data = rand()%100;
            DListPushFront((dll_t *)dll, &data);
            sem_post(&semaphore);
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

void *Consume(void *dll)
{
    int res = 0;
    while(1)
    {
        sem_wait(&semaphore);
        pthread_mutex_lock(&lock);

        ++con_count;
        res = *(int *)DListPopBack((dll_t *)dll);
        if(res == poison_apple_g)
        {
            pthread_mutex_unlock(&lock);
            break;
        }
        res_g += res;
        printf("current res: %d\n", res);

        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main()
{
    pthread_t producer[SIZE];
    pthread_t consumer[SIZE];

    int err_no = 0;
    int i = 0;

    dll_t *dll = NULL;
    dll = DListCreate();

    pthread_mutex_init(&lock, NULL);
    sem_init(&semaphore, 0, 0);

    for(i=0; i < SIZE; i++)
    {
        err_no = pthread_create(producer+i, NULL, Produce, (void *)dll);
        err_no = pthread_create(consumer+i, NULL, Consume, (void *)dll);
    }
    
    sleep(3);
    shut_down_g = 1;

    for(i = 0; i < SIZE; i++)
    {
        pthread_join(producer[i], NULL);
    }

    pthread_mutex_lock(&lock);
    for(i=0; i<SIZE; i++)
    {
        DListPushFront(dll, &poison_apple_g);
        sem_post(&semaphore);
    }
    pthread_mutex_unlock(&lock);

    for(i = 0; i < SIZE; i++)
    {
        pthread_join(consumer[i], NULL);
    }

 
    printf("total res: %d\n",res_g);
    printf("producer counter: %d\n",pro_count);
    printf("consumer counter: %d\n",con_count);

    pthread_mutex_destroy(&lock);
    sem_destroy(&semaphore);
    DListDestroy(dll);

    return 0;
}
#endif
/*------------------------------- EXERCISE 4: -------------------------------*/
#if 0
#define SIZE 5
#define QSIZE 40

pthread_mutex_t lock;
sem_t sem_ready;
sem_t sem_avilable;

int res_g = 0;
int poison_apple_g = 770;
int shut_down_g = 0;
int pro_count = 0;
int con_count = 0;

void *Produce(void *cb)
{
    int data = 0;
    while(1)
    {
        sem_wait(&sem_avilable);
        pthread_mutex_lock(&lock);
        ++pro_count;
        if(1 == shut_down_g)
        {  
            pthread_mutex_unlock(&lock);
            break;

        }
        else
        {
            data = rand()%100;
            CircBuffWrite((circ_buff_t *)cb, &data, sizeof(data));
            sem_post(&sem_ready);
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

void *Consume(void *cb)
{
    int res = 0;
    while(1)
    {
        sem_wait(&sem_ready);
        pthread_mutex_lock(&lock);

        ++con_count;
        CircBuffRead(cb, &res, sizeof(res));
        if(res == poison_apple_g)
        {
            sem_post(&sem_avilable);
            pthread_mutex_unlock(&lock);
            break;
        }
        sem_post(&sem_avilable);
        res_g += res;

        pthread_mutex_unlock(&lock);
        printf("current res: %d\n", res);
    }
    return NULL;
}

int main()
{
    pthread_t producer[SIZE];
    pthread_t consumer[SIZE];

    int err_no = 0;
    int i = 0;

    circ_buff_t *cb = NULL;
    cb = CircBuffCreate(QSIZE);

    pthread_mutex_init(&lock, NULL);
    sem_init(&sem_ready, 0, 0);
    sem_init(&sem_avilable, 0, QSIZE);



    for(i=0; i < SIZE; i++)
    {
        err_no = pthread_create(producer+i, NULL, Produce, (void *)cb);
        err_no = pthread_create(consumer+i, NULL, Consume, (void *)cb);
    }
    
    sleep(3);
    shut_down_g = 1;

    for(i = 0; i < SIZE; i++)
    {
        pthread_join(producer[i], NULL);
    }

    for(i=0; i<SIZE; i++)
    {
        sem_wait(&sem_avilable);
        pthread_mutex_lock(&lock);
        CircBuffWrite(cb, &poison_apple_g, sizeof(poison_apple_g));
        sem_post(&sem_ready);
        pthread_mutex_unlock(&lock);
    }

    for(i = 0; i < SIZE; i++)
    {
        pthread_join(consumer[i], NULL);
    }

 
    printf("total res: %d\n",res_g);
    printf("producer counter: %d\n",(pro_count-con_count));


    pthread_mutex_destroy(&lock);
    sem_destroy(&sem_avilable);
    sem_destroy(&sem_ready);
    CircBuffDestroy(cb);

    return 0;
}
#endif
/*------------------------------- EXERCISE 5: -------------------------------*/
#if 0
#define SIZE 5

pthread_mutex_t lock_pord;
pthread_mutex_t lock_cons;
sem_t sem_ready;
sem_t sem_avilable;

int res_g = 0;
int poison_apple_g = 770;
int shut_down_g = 0;
int pro_count = 0;
int con_count = 0;

void *Produce(void *cb)
{
    int data = 0;
    (void)cb;
    while(1)
    {
        sem_wait(&sem_avilable);
        pthread_mutex_lock(&lock_pord);
        ++pro_count;
        if(1 == shut_down_g)
        {  
            pthread_mutex_unlock(&lock_pord);
            break;

        }
        else
        {
            data = rand()%100;
            CircularFixedBufferWrite(cf_buff, data);
            sem_post(&sem_ready);
            pthread_mutex_unlock(&lock_pord);
        }
    }
    return NULL;
}

void *Consume(void *cb)
{
    int res = 0;
    (void)cb;
    while(1)
    {
        sem_wait(&sem_ready);
        pthread_mutex_lock(&lock_cons);

        ++con_count;
        res = CircularFixedBufferRead(cf_buff);
        if(res == poison_apple_g)
        {
            sem_post(&sem_avilable);
            pthread_mutex_unlock(&lock_cons);
            break;
        }
        sem_post(&sem_avilable);
        res_g += res;

        pthread_mutex_unlock(&lock_cons);
        printf("current res: %d\n", res);
    }
    return NULL;
}

int main()
{
    pthread_t producer[SIZE];
    pthread_t consumer[SIZE];

    int err_no = 0;
    int i = 0;  
    
    pthread_mutex_init(&lock_pord, NULL);
    pthread_mutex_init(&lock_cons, NULL);
    
    sem_init(&sem_ready, 0, 0);
    sem_init(&sem_avilable, 0, BUFFSIZE);

    for(i=0; i < SIZE; i++)
    {
        err_no = pthread_create(producer+i, NULL, Produce, NULL);
        err_no = pthread_create(consumer+i, NULL, Consume, NULL);
    }
    
    sleep(3);
    shut_down_g = 1;

    for(i = 0; i < SIZE; i++)
    {
        pthread_join(producer[i], NULL);
    }

    for(i=0; i<SIZE; i++)
    {
        sem_wait(&sem_avilable);
        pthread_mutex_lock(&lock_pord);
        CircularFixedBufferWrite(cf_buff, poison_apple_g);
        sem_post(&sem_ready);
        pthread_mutex_unlock(&lock_pord);
    }

    for(i = 0; i < SIZE; i++)
    {
        pthread_join(consumer[i], NULL);
    }

 
    printf("total res: %d\n",res_g);
    printf("number of un finished threads: %d\n",(pro_count-con_count));
    printf("producer counter: %d\n",pro_count);
    printf("consumer counter: %d\n",con_count);


    pthread_mutex_destroy(&lock_cons);
    pthread_mutex_destroy(&lock_pord);
    sem_destroy(&sem_avilable);
    sem_destroy(&sem_ready);

    return 0;
}
#endif
/*------------------------------- EXERCISE 6: -------------------------------*/
#if 1
#define SIZE 5

pthread_mutex_t lock;
sem_t sem;
pthread_cond_t cv;

int msg = 0;
int poison_apple_g = -1;
int shut_down_g = 0;
int pro_count = 0;
int con_count = 0;

__thread int ii;
__thread int jj = 6;

void *Produce(void *cb)
{
    int data = 0;
    int i = 0;
    (void)cb;
    while(1)
    {
        data = rand()%100;
        for(i=0;i<SIZE;++i)
        {
            sem_wait(&sem);
        }
        pthread_mutex_lock(&lock);
        ++pro_count;
        if(1 == shut_down_g)
        {  
            pthread_mutex_unlock(&lock);
            break;
        }
        msg = data;
        pthread_mutex_unlock(&lock);
        printf("msg was sent: %d\n", msg);
        pthread_cond_broadcast(&cv);
    }
    return NULL;
}

void *Consume(void *cb)
{
    int res = 0;
    (void)cb;
    while(1)
    {
        pthread_mutex_lock(&lock);
        sem_post(&sem);
        pthread_cond_wait(&cv, &lock);
        ++con_count;
        res = msg;
        pthread_mutex_unlock(&lock);
        printf("msg recived: %d\n", res);
        if(res == poison_apple_g)
        {
            break;
        }
    }
    return NULL;
}

int main()
{
    pthread_t producer;
    pthread_t consumer[SIZE];

    int err_no = 0;
    int i = 0;  

    pthread_mutex_init(&lock, NULL);
    sem_init(&sem, 0, 0);
    pthread_cond_init(&cv,NULL);
    
    err_no = pthread_create(&producer, NULL, Produce, (void *)&msg);

    for(i=0; i < SIZE; i++)
    {
        err_no = pthread_create(consumer+i, NULL, Consume, (void *)&msg);
    }
    
    sleep(3);
    shut_down_g = 1;

    pthread_join(producer, NULL);

    msg = poison_apple_g;
    pthread_cond_broadcast(&cv);

    for(i = 0; i < SIZE; i++)
    {
        pthread_join(consumer[i], NULL);
    }
 
    if(pro_count*SIZE != con_count)
    {
        printf("ERROR on counter:\n");
        printf("producer counter: %d\n",pro_count*SIZE);
        printf("consumer counter: %d\n",con_count);
    }

    pthread_mutex_destroy(&lock);
    sem_destroy(&sem);
    pthread_cond_destroy(&cv);

    return 0;
}
#endif