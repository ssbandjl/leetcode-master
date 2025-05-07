#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;  // Mutex initialized statically

void *thread_function(void *arg) {
    pthread_mutex_lock(&lock);  // Lock the mutex
    
    // Critical section code
    printf("Thread %ld has entered the critical section.\n", (long)arg);
    
    pthread_mutex_unlock(&lock);  // Unlock the mutex
    return NULL;
}

int main() {
    pthread_t threads[2];

    // Create two threads
    for (long i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void *)i);
    }

    // Wait for both threads to finish
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
