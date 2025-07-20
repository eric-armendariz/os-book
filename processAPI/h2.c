#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

// can you write to file from child and parent processes?
int main(int argc, char *argv[]) {
    close(STDOUT_FILENO);
    int fd = open("./h2.output", O_CREAT|O_WRONLY|O_TRUNC|S_IRWXU);

    int fc = fork();
    if (fc < 0) {
        printf("failed to fork\n");
        exit(1);
    } else if (fc == 0) {
        char *data = "hello from child\n";
        write(fd, data, strlen(data));
    } else {
        char *data = "hello from parent\n";
        write(fd, data, strlen(data));
    }

    return 0;
}