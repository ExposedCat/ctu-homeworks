#include <pthread.h>
#include <stdio.h>

// Docs
void* thread_worker(void* id);
pthread_t run_new_thread(char* id);
void print_thread_id(char* id);

// MAIN
int main() {
    pthread_t thread_id;
    run_new_thread("0");
    print_thread_id("the main thread");
    run_new_thread("1");
    run_new_thread("2");
    return 0;
}

// Services
void print_thread_id(char* id) {
    printf("Thread Running. I am %s\n", id);  //
}

void* thread_worker(void* id) {
    print_thread_id((char*)id);
    return NULL;
}

pthread_t run_new_thread(char* id) {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, thread_worker, id);
    pthread_join(thread_id, NULL);
    return thread_id;
}