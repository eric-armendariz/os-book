#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
// Using linux libaries

volatile int total = 0;
int loops;

void *my_thread_function(void *arg) {
    for (int i = 0; i < loops; i++) {
        total++;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("enter a loop number");
        exit(1);
    }
    loops = atoi(argv[1]);
    pthread_t my_thread;
    pthread_t my_thread_2;
    pthread_create(&my_thread, NULL, my_thread_function, NULL);
    pthread_create(&my_thread_2, NULL, my_thread_function, NULL);
    pthread_join(my_thread, NULL);
    pthread_join(my_thread_2, NULL);
    printf("Hello from the main thread!\n");
    printf("%d", total);
    return 0;
}