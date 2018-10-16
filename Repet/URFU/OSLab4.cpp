#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

const int maxbuf = 5;
typedef char elem;

elem buf[maxbuf];

int curpos = 0;
int datasz = 0;

int cnt_thread = 42;

pthread_mutex_t pmutex;
pthread_rwlock_t rwlock;

void * reader_f(void * arg)
{
    int num = (intptr_t) arg;
    while (1)
    {
        pthread_rwlock_rdlock(&rwlock);
        pthread_mutex_lock(&pmutex);
        if (datasz > 0)
            break;
        pthread_mutex_unlock(&pmutex);
        pthread_rwlock_unlock(&rwlock);
        usleep(10);
    }
    int readfrom = curpos;
    --datasz;
    curpos = (curpos + 1) % maxbuf;
    int s3 = datasz;
    int s1 = curpos;
    int s2 = readfrom;
    pthread_mutex_unlock(&pmutex);
    elem c = buf[readfrom];
    printf("[%3d] read %c %d %d %d\n", num, c, s1, s2, s3);
    //sleep((num * 997) % 2); //read matters
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}

void * writer_f(void * arg)
{
    int num = (intptr_t) arg;
    while (1)
    {
        pthread_rwlock_wrlock(&rwlock);
        if (datasz < maxbuf)
            break;
        pthread_rwlock_unlock(&rwlock);
        usleep(10);
    }
    int writeto = (curpos + datasz) % maxbuf;
    buf[writeto] = 'A' + num / 2;
    ++datasz;
    int s3 = datasz;
    int s1 = curpos;
    int s2 = writeto;
    printf("[%3d] writ %c %d %d %d\n", num, 'A' + num / 2, s1, s2, s3);
    //sleep((num * 997) % 2); //write matters
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t threads[cnt_thread];
    pthread_mutex_init(&pmutex, NULL);
    pthread_rwlock_init(&rwlock, NULL);
    for (size_t i = 0; i < cnt_thread; ++i)
    {
        if (i % 2)
            pthread_create(&threads[i], NULL, writer_f, (void *)i);
        else
            pthread_create(&threads[i], NULL, reader_f, (void *)i);
    }
    for (size_t i = 0; i < cnt_thread; ++i)
    {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&pmutex);
    pthread_rwlock_destroy(&rwlock);
    printf("---end---\n");
    return 0;
}
