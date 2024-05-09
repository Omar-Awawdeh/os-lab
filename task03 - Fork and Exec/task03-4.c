#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    pid_t pid1, pid2, pid3, pid4, pid5, pid6;

    printf("Parent process with \t\tPID: %d\tPPID: %d\n", getpid(), getppid());

    // First child process
    pid1 = fork();

    if (pid1 == 0)
    {
        printf("First child process with \tPID: %d\tPPID: %d\n", getpid(), getppid());
        exit(0);
    }

    // Second child process
    pid2 = fork();

    if (pid2 == 0)
    {
        printf("Second child process with \tPID: %d\tPPID: %d\n", getpid(), getppid());

        // Child process of the second child
        pid3 = fork();

        if (pid3 == 0)
        {
            printf("Third child process with \tPID: %d\tPPID: %d\n", getpid(), getppid());
            exit(0);
        }

        waitpid(pid3, NULL, 0);
        exit(0);
    }

    // Fourth child process
    pid4 = fork();

    if (pid4 == 0)
    {
        printf("Fourth child process with \tPID: %d\tPPID: %d\n", getpid(), getppid());

        // Fifth child process, child of the fourth child
        pid5 = fork();

        if (pid5 == 0)
        {
            printf("Fifth child process with \tPID: %d\tPPID: %d\n", getpid(), getppid());

            // Child process of the fifth child
            pid6 = fork();
            if (pid6 == 0)
            {
                printf("Sixth child process with \tPID: %d\tPPID: %d\n", getpid(), getppid());
                exit(0);
            }
            waitpid(pid6, NULL, 0);
            exit(0);
        }

        waitpid(pid5, NULL, 0);
        exit(0);
    }

    // wait for all childs to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid4, NULL, 0);

    return 0;
}
