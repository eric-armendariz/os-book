#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int *val = malloc(sizeof(int));
    *val = 0;
    printf("starting process with id: %d\n", getpid());
    printf("initial values: %d\n", *val);
    
    int fc = fork();
    if (fc < 0) {
        printf("failed to fork\n");
        exit(1);
    } else if (fc == 0) {
        *val = 100;
        printf("child variable value: %d\n", *val);
    } else {
        wait(NULL);
        printf("parent variable value: %d\n", *val);
    }

    free(val);
    return 0;
}