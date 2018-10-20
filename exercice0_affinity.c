#define _GNU_SOURCE

#include <pthread.h>

#define NB_THREADS 4

void change_affinity(int cpu, pthread_t thread) {
    cpu_set_t mask; 
    CPU_ZERO( &mask ); 
    CPU_SET( cpu, &mask ); 
    pthread_setaffinity_np(thread, sizeof(mask), &mask);   
}

void mange_cpu(void) {
    /*
     On mange du CPU en mettant du code qui ne devrait pas
     etre supprimé par le compilateur
     */
	int i=1;
	while (1) {
		i=i*2+1;
		if (i%2==0){
			break;
		}
  	} 
}

void *threadMethod(void *vargp){
	change_affinity(1,pthread_self());
	mange_cpu();
}

int main() 
{
	pthread_t threads[NB_THREADS];
	for(int i=0; i< NB_THREADS; i++){
    	pthread_create(threads+i, NULL, threadMethod, NULL);
	}
	for(int i=0; i< NB_THREADS; i++){
    	pthread_join(threads+i, NULL);
	}
    return 0; 
}
