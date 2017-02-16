#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t queue_not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t queue_free_places = PTHREAD_COND_INITIALIZER;

int queue[SIZE];
int qlen = 0;

void *producer( void *p)
{
    int r;

    while(1) {
        r = rand() % 10;
        //sleep(1);
        printf( "generated: %d\n", r);

        pthread_mutex_lock( &mutex);
        while(qlen == SIZE) {
            pthread_cond_wait( &queue_free_places, &mutex);
        }

        queue[qlen] = r;
        qlen++;
        pthread_cond_broadcast( &queue_not_empty);
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}


int do_something_complex(int r)
{
    sleep(1);
    printf( "square of %d is %d\n", r, r*r);
}

void *consumer( void *p)
{
    int r;

    while(1) {

        pthread_mutex_lock( &mutex);
        while( qlen == 0) {
            pthread_cond_wait( &queue_not_empty, &mutex);
        }

        r = queue[0];
        qlen--;
        if( qlen)
            memmove(queue, queue+1, qlen*sizeof(int));
        printf( "received: %d\n", r);
        pthread_cond_broadcast(&queue_free_places);
        pthread_mutex_unlock(&mutex);

        do_something_complex(r);
    }

    return 0;
}

int main()
{
    pthread_t t1, t2, t3;

    pthread_create( &t1, 0, producer, 0);
    pthread_create( &t2, 0, consumer, 0);
    pthread_create( &t3, 0, consumer, 0);
    pthread_create( &t3, 0, consumer, 0);
    pthread_create( &t3, 0, consumer, 0);
    pthread_create( &t3, 0, consumer, 0);
    pthread_create( &t3, 0, consumer, 0);

    pthread_join(t1, 0);
    pthread_join(t2, 0);
    pthread_join(t3, 0);

    return 0;
}


