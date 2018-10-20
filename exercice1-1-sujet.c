#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>
#include <math.h>


#define NB_THREADS 4


unsigned long x, y=362436069, z=521288629;
uint64_t results[NB_THREADS];

void change_affinity(int cpu, pthread_t thread) {
    cpu_set_t mask; 
    CPU_ZERO( &mask ); 
    CPU_SET( cpu, &mask ); 
    pthread_setaffinity_np(thread, sizeof(mask), &mask);   
}

unsigned long xorshf96() {
    unsigned long t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;
    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;
    return z;
}

void print_odd(int id, uint64_t res) {
    printf("# of odd numbers in Thread%d: %d\n", id, res);
}

void *fct(void *args) {
    int id = (int)args;
    change_affinity(id, pthread_self());
    struct timeval time;
	gettimeofday (&time, NULL);
	x = (unsigned long int) time.tv_usec * (id + 1);
	for (int i = 0; i < pow(10,8); ++i) {
		unsigned long t;

		x ^= x << 16;
		x ^= x >> 5;
		x ^= x << 1;

		t = x;
		x = y;
		y = z;
		z = t ^ x ^ y;
        
        results[id] += (z%2);
	}
}

int main(int argc, char const *argv[])
{
    pthread_t threads[NB_THREADS];
    for(int i=0; i < NB_THREADS; i++) {
        pthread_create(threads + i, NULL, fct, i);
    }
    for(int i=0; i < NB_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    for(int i=0; i < NB_THREADS; i++) {
        printf("# of odd numbers in Thread%d: %d\n", i, results[i]);
    }
    return 0;
}
