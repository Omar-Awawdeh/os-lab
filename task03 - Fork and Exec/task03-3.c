#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    char command[20];

    while (1)
    {
        scanf("%s", command);

        if (strcmp(command, "exit") == 0)
            exit(0);

        pid_t pid = fork();
        if (pid == 0)
        {
            execlp(command, command, NULL);
            exit(0);
        }
        else
        {
            wait(NULL);
        }
    }

    return 0;
}