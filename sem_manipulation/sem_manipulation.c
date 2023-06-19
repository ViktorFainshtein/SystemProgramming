#include <stdio.h>  /*printf*/
#include <pthread.h>    /*pthread*/
#include <semaphore.h>  /*sem_t*/
#include <unistd.h>
#include <stdlib.h> /*atoi*/

#define BUFF_SIZE 10

sem_t semaphore;

int main()
{
    char buffer[BUFF_SIZE] = {'0'};
    int num = 0;
    /*0 indicates it will be shated between threads and not processes*/
    /*4 is the set initial value of the semaphore*/
    sem_init(&semaphore, 0, 4);

    while (1)
    {
        fgets(buffer, 9, stdin);
        
        switch (buffer[0])
        {
            case 'D':
                num = atoi(buffer+2);
                while(num)
                {
                    sem_trywait(&semaphore);
                    --num;
                }
                break;
            case 'I':
                num = atoi(buffer+2);
                while (num)
                {
                    sem_post(&semaphore);
                    --num;
                }            
                break;
            case 'V':
                sem_getvalue(&semaphore, &num);
                printf("The Value of the semaphore is: %d\n", num);
                break;
            case 'X':
                sem_destroy(&semaphore);
                return 0;
                break;
            default:
                break;
        }
    }

    return 0;
}