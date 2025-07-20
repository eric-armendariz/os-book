#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    printf("starting process with pid: %d\n", getpid());
    int rc = fork();
    
    if (rc < 0) {
        printf("fork failed");
        exit(1);
    } else if (rc == 0) {
        printf("hello from child process: %d\n", getpid());
        close(STDOUT_FILENO);
        open("./fork.output", O_CREAT|O_WRONLY|O_TRUNC|S_IRWXU);
        char *args[3];
        args[0] = "wc";
        args[1] = "wc.txt";
        args[2] = NULL;
        int execVal = execvp(args[0], args);
        printf("value returned from execVal: %d\n", execVal);
    } else {
        int wait_rc = waitpid(rc, NULL, 0);
        printf("hello from parent process: %d\n just finished waiting for child process: %d\n waitpid(%d, NULL) returned: %d\n", getpid(), rc, rc, wait_rc);
    }
    return 0;
}