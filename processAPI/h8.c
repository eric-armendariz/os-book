// Pipe output of one child process to another

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int pipefd[2];
    char buf;
    if (argc != 2) {
        printf("add an arg to pipe between processes\n");   
        exit(1);
    }
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }
    
    int fc = fork();
    if (fc < 0) {
        printf("failed to fork\n");
        exit(1);
    } else if (fc == 0) {
        // write to pipe
        close(pipefd[0]);
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]);
    } else {
        wait(NULL);
        // read from pipe
        close(pipefd[1]);
        while (read(pipefd[0], &buf, 1) > 0) {
            printf("%c", buf);
        }
        printf("\n");
        close(pipefd[0]);
    }

    return 0;
}