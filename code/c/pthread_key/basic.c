#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// total thread count
#define N 8

pthread_t g_threads[N];
pthread_key_t g_key;

void* thread_func(void* arg)
{
    int n, *p;
    
    n = rand() % 100;
    p = NULL;
    printf("thread %lx set: %d\n", pthread_self(), n);
    pthread_setspecific(g_key, &n);
    usleep(100);
    p = pthread_getspecific(g_key);
    printf("thread %lx get: %d\n", pthread_self(), *p);

    return NULL;
}

int main(int argc, char** argv)
{
    int i;

    printf("thread count: %d\n\n", N);
    srand(time(NULL));
    pthread_key_create(&g_key, NULL);

    for (i = 0; i < N; i++)
        pthread_create(&g_threads[i], NULL, thread_func, NULL);
    for (i = 0; i < N; i++)
        pthread_join(g_threads[i], NULL);

    return 0;
}
