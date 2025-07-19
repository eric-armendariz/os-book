#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <assert.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Incorrect usage expected 2 arguments");
        return -1;
    }
    int *x = malloc(sizeof(int));
    printf("Pointer address: %p", x);
    printf("Process ID: %d", GetCurrentProcessId());
    *x = 0;
    while (1) {
        Sleep(1000);
        printf("%d\n", *x);
        *x = *x + 1;
    }
    return 0;
}