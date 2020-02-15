#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main ()
{
    pid_t pid;
    int status;

    pid = fork();
    
    if (pid < 0) { // error 
        fprintf(stderr, "fork failed \n");
        exit(1);
    }
    else if (pid == 0) { // child process
        execlp("/bin/ls", "ls", NULL);
    }
    else { // parent process
        wait(&status);
        printf("child completed \n");
        printf("child exit status: %d \n", status);
    }

    exit(0);
}