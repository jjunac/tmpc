#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>
#include <math.h>


#define NB_THREADS 4

__thread unsigned long x, y=362436069, z=521288629;
// Bottleneck ??
__thread uint64_t results;

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

void *fct(void *args) {
    int id = (int)args;
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
        
        results += (z%2);
	}
    printf("# of odd numbers in Thread%d: %d\n", id, results);
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
    // Je pense que le bottleneck c'est le tableau de results, mais pas d'amélioration chez moi
    // Default: 6s
    // O1: 4s
    // O2: 3s
    // O3: 3s
    // for(int i=0; i < NB_THREADS; i++) {
    //     printf("# of odd numbers in Thread%d: %d\n", i, results[i]);
    // }
    return 0;
}
