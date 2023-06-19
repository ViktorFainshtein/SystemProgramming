#define _POSIX_C_SOURCE 199309L

#include <stdio.h> /*printf*/
#include <stdlib.h> /*qsort*/
#include <pthread.h> /*pthread_t*/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /*mmap*/
#include <fcntl.h>
#include <string.h> /*memchr strlen*/
#include <time.h> /*clock_gettime*/
#include <unistd.h>

#define NUM_OF_THREADS 2
#define BILLION  1000000000L

typedef struct data
{
    char **begin;
    char **end;
    size_t size;
}data_t;

int ShuffleCompareFunction(const void *arr1, const void *arr2);
int SortCompareFunction(const void *arr1, const void *arr2);
void PrintPointerArr(char **arr, size_t size);
void PrintBuffer(char *arr, size_t size);
void *Sort(void *data);
data_t InitData(data_t data, char **begin, size_t size);
char **Merge(char **arr, size_t size, data_t data[]);
char *FindMin(data_t *data);

int main()
{
    int fdin = 0, size = 0, num_of_words = 0;
    int i = 0, num_of_dicts = 5, temp = 0;

    char *buffer = NULL;
    char *wc_buffer = NULL;
    char *wc_buf_temp = NULL;
    char **words_arr = NULL;
    char **temp_words_arr = NULL;

    struct stat statbuf;
    struct timespec start, finish;
    double time_diff = 0;

    pthread_t threads[NUM_OF_THREADS];
    data_t data[NUM_OF_THREADS];

    /* open the input file */
    fdin = open ("/usr/share/dict/words", O_RDONLY);
    if(fdin < 0)
    {
        printf("can't open dictionary for reading\n");
        return 0;
    }

    /* find size of input file */
    if(0 > fstat(fdin, &statbuf))
    {
        printf ("fstat error");
        return 0;
    }
    size = statbuf.st_size;

    /*set the file into a buffer*/
    buffer = (char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fdin, 0);
    
    wc_buffer = buffer;
    wc_buf_temp = buffer + size;

    /*count the number of words and replace "/n" with "/0"*/
    while(wc_buffer = memchr(wc_buffer, '\n', wc_buf_temp - wc_buffer))
    {
        *wc_buffer = '\0';
        ++wc_buffer;
        ++num_of_words;
    }
    
/*     wc_buffer = buffer;
    PrintBuffer(wc_buffer, num_of_words); */

    words_arr = (char **)malloc(sizeof(char *) * num_of_words * num_of_dicts);
    wc_buffer = buffer;

    /*init the dynamicaly allocated array*/
    for(i = 0; i < (num_of_dicts * num_of_words); ++i)
    {
        if(buffer + size == wc_buffer)
        {
            wc_buffer = buffer;
        }
        words_arr[i] = wc_buffer;
        wc_buffer += (strlen(wc_buffer)+1);
    }

    /*Suffle well the arr*/
    for(i = 0; i < 25 ; ++i)
    {
        qsort(words_arr, (num_of_dicts * num_of_words), sizeof(char *), ShuffleCompareFunction);
    }


    temp_words_arr = words_arr;

    /*init data structr before sending to threads*/
    for(i = 0; i < NUM_OF_THREADS; ++i)
    {
        data[i] = InitData(data[i], temp_words_arr+(i*((num_of_words * num_of_dicts)/NUM_OF_THREADS)), (num_of_words * num_of_dicts));
    }
    
    /*in case of carry*/
    data[NUM_OF_THREADS-1].size += ((num_of_words * num_of_dicts)%NUM_OF_THREADS);
    data[NUM_OF_THREADS-1].end = data[NUM_OF_THREADS-1].begin + data[NUM_OF_THREADS-1].size;

/*     PrintPointerArr(words_arr, num_of_words); */

    clock_gettime(CLOCK_REALTIME, &start);
    for(i = 0; i < NUM_OF_THREADS; ++i) 
    {
        pthread_create(&threads[i], NULL, Sort, (void *)&data[i]);
    }
    for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }

/*     PrintPointerArr(words_arr, num_of_words*num_of_dicts); */

    temp_words_arr = Merge(words_arr, (num_of_words * num_of_dicts), data);
    clock_gettime(CLOCK_REALTIME, &finish);

    PrintPointerArr(temp_words_arr, num_of_words * num_of_dicts);
/*     PrintPointerArr(words_arr, (num_of_dicts * num_of_words)); */

    /*sort the shuffled arr*/
    time_diff = (double)(finish.tv_sec - start.tv_sec) + ((double)(finish.tv_nsec - start.tv_nsec)/BILLION);
    printf("Time: %lf\n", time_diff);



    return 0;
}

int ShuffleCompareFunction(const void *arr1, const void *arr2)
{
    int num = 0;

    num = rand() % 30;
    if(num > 20 && num < 30)
    {
        return 1;
    }
    else if(num > 10 && num < 20)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int SortCompareFunction(const void *arr1, const void *arr2)
{
    return strcmp(*(char **)arr1, *(char **)arr2);
}

void PrintPointerArr(char **arr, size_t size)
{
    int i = 0;
    for (i = 0; i < (size); ++i)
    {
        printf("%s i = %d\n", arr[i], i);
    }
}

void PrintBuffer(char *arr, size_t size)
{
    int i = 0;
    for(i = 0; i < size; ++i)
    {
        printf("%s\n", arr);
        arr += (strlen(arr) + 1);
    }
}

void *Sort(void *data)
{
    qsort(((data_t *)data)->begin, ((data_t *)data)->size, sizeof(char *), SortCompareFunction);
    return NULL;
}

data_t InitData(data_t data, char **begin, size_t size)
{
    data.begin = begin;
    data.end = (begin + (size/NUM_OF_THREADS));
    data.size = (size / NUM_OF_THREADS);
    return data;
}

char **Merge(char **arr, size_t size, data_t data[])
{
    char **new_arr = NULL;
    char **temp = NULL; 
    data_t *dynamin_data = NULL;
    size_t i = 0;

    new_arr = (char **)malloc(sizeof(char *) * size);
    dynamin_data = (data_t*)malloc(sizeof(data_t) * NUM_OF_THREADS);
    
    temp = new_arr;

    for (i = 0; i < NUM_OF_THREADS; i++)
    {
        dynamin_data[i].begin = data[i].begin;
        dynamin_data[i].end = data[i].end;
        dynamin_data[i].size = data[i].size;
    }
    
    for (i = 0; i < size; i++)
    {
        *temp = FindMin(dynamin_data);
        ++temp;
    }

    return new_arr;
}

char *FindMin(data_t *data)
{
    char *min = NULL;
    int i = 0;

    for (i = 0; i < NUM_OF_THREADS; i++)
    {
        if(data[i].begin == data[i].end)
        {
            continue;
        }
        min = *(data[i].begin);
    }

    for (i = 0; i < NUM_OF_THREADS; i++)
    {
        if(data[i].begin == data[i].end)
        {
            continue;
        }
        if(strcmp(*(data[i].begin), min) < 0)
        {
            min = *(data[i].begin);
        }
    }

    for (i = 0; i < NUM_OF_THREADS; i++)
    {
        if(data[i].begin == data[i].end)
        {
            continue;
        }
        if(strcmp(*(data[i].begin), min) == 0)
        {
            ++data[i].begin;
            break;
        }
    }

    return min;
}