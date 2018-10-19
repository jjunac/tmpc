#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <sched.h>
#include <sys/time.h>
#include <math.h>


#define NB_THREADS 4

struct results {
    uint64_t result0;
    char padding1[PADDING];
    uint64_t result1;
    char padding2[PADDING];
    uint64_t result2;
    char padding3[PADDING];
    uint64_t result3;
} __attribute__ ((packed)) results __attribute__ ((aligned (64)));;

__thread unsigned long x, y=362436069, z=521288629;

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
        
        // results += (z%2)

        switch (id)
        {
            case 0:
                results.result0 += (z%2);
                break;
            case 1:
                results.result1 += (z%2);
                break;
            case 2:
                results.result2 += (z%2);
                break;
            case 3:
                results.result3 += (z%2);
                break;
        }
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
    printf("%d, %d, %d, %d\n", PADDING, results.result0, results.result1, results.result2, results.result3);
    return 0;
}
