#include <limits.h>
#include <pthread.h>
#include <stdio.h>

#define length 10

// Global
pthread_mutex_t max_lock;
int max = INT_MIN;

// Docs
void* thread_worker(void* data);
pthread_t run_new_thread(void* data);
void join_threads(pthread_t threads[length]);

// MAIN
int main() {
    pthread_t threads[length];
    int numbers[length] = {1, 7, -1, 10, -1, 55, 4, 8, 10, 4};
    for (size_t i = 0; i < length; ++i) {
        pthread_t thread_id = run_new_thread(&numbers[i]);
        threads[i] = thread_id;
    }
    join_threads(threads);
    printf("The biggest number was %d\n", max);
    return 0;
}

// Services
void* thread_worker(void* data) {
    pthread_mutex_lock(&max_lock);
    int number = *(int*)data;
    if (number > max) {
        max = number;
    }
    pthread_mutex_unlock(&max_lock);
    return NULL;
}

void join_threads(pthread_t threads[length]) {
    for (size_t i = 0; i < length; ++i) {
        pthread_join(threads[i], NULL);
    }
}

pthread_t run_new_thread(void* data) {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, thread_worker, data);
    return thread_id;
}