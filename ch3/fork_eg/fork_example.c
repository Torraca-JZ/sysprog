#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main ()
{
    pid_t pid;

    pid = fork();
    
    if (pid < 0) { // error 
        fprintf(stderr, "fork failed \n");
        exit(1);
    }
    else if (pid == 0) { // child process
        execlp("/bin/ls", "ls", NULL);
    }
    else { // parent process
        wait(NULL);
        printf("child completed \n");
    }

    exit(0);
}