#include <sys/types.h>
#include <sys/wait.h>
// #include <sys/shm.h>
// #include <sys/stat.h>
#include <sys/mman.h>
// #include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static int *test;


int main () 
{
    pid_t pids[5];
    int i;
    int n = 5;


    test = mmap(NULL, sizeof(*test), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *test = 1;

    /* Start children. */
    for (i = 0; i < n; ++i) {
        if ((pids[i] = fork()) < 0) {
            perror("fork");
            abort();
        } else if (pids[i] == 0) {
            printf("child %d, pid %d \n", pids[i], getpid());
            printf("test: %d\n", *test);
            *test++;
            printf("test: %d\n", *test);
        if (msync(test, sizeof(int), MS_SYNC) < 0) {
            perror("msync failed");
            exit(1);
        }
            exit(0);
        }
    }

    /* Wait for children to exit. */
    printf("Parent waiting\n");
    int status;
    pid_t pid;
    while (n > 0) {
        pid = wait(&status);
        printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
        --n;  // TODO(pts): Remove pid from the pids array.
    }

    exit(0);
}