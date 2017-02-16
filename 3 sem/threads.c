#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


double function(double x)
{
    return x*x*x;
}

struct Task {
    double from;
    double to;
    int n;
    double s;
};


void* my_thread (void * arg)
{
    struct Task *t = (struct Task *)arg;
    int i;

    double delta = (t->to - t->from)/t->n;
    double s = 0;
    double x = t->from;
    double f_ot_x = function(x);

    for(i=0; i< t->n; i++) {
        double old_f = f_ot_x;
        f_ot_x = function(x+delta);
        s += (old_f + f_ot_x)/2*delta;
        x += delta;
    }

    printf( "Partial integration on interval [%f,%f] = %f\n", t->from, t->to, s);

    t->s = s;

    return 0;
}

int main(int argc, char **argv)
{
    int ret;
    pthread_t *t;
    void* retval;
    int i;

    double from;
    double to;
    double delta;
    int n;

    sscanf( argv[1], "%lf", &from);
    sscanf( argv[2], "%lf", &to);
    sscanf( argv[3], "%d", &n);

    int numCPU = sysconf( _SC_NPROCESSORS_ONLN );


    printf( "Number of processors: %d\n",  numCPU);
    printf( "Integrating from %lf to %lf (n=%d)\n", from, to, n);
    delta = (to - from)/numCPU;

    struct Task *tasks = malloc( sizeof(struct Task)*numCPU);
    t = malloc(sizeof(pthread_t)*numCPU);

    for(i=0; i<numCPU; i++) {
        tasks[i].n = n;
        tasks[i].from = from + i*delta;
        tasks[i].to = tasks[i].from + delta;
        ret = pthread_create( t+i, 0, my_thread, tasks + i);
    }

    double integral = 0;

    for(i=0; i<numCPU; i++) {
        pthread_join(t[i], &retval);
        integral += tasks[i].s;
    }

    printf( "Result: %lf\n", integral);
    return 0;
}