#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        printf("From child process \tPID : %d\t PPID : %d\n", getpid(), getppid());
    }
    else
    {
        wait(NULL);
        printf("From parent process \tPID : %d\t PPID : %d\n", getpid(), getppid());
    }

    return 0;
}