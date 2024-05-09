#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>
#include <string.h>

void main()
{
    int p1[2];
    int p2[2];
    char word[11];

    if (pipe(p1) == -1 || pipe(p2) == -1)
    {
        printf("Failed to create pipes\n");
        return;
    }

    pid_t pid1 = fork();
    if (pid1 == -1)
    {
        printf("Failed to create child process 1\n");
        return;
    }
    else if (pid1 == 0)
    {
        // Child 1, writes on p1[1] and reads from p2[0]
        close(p1[0]);
        close(p2[1]);

        while (1)
        {
            scanf("%s", word);
            write(p1[1], word, 11);

            if (strcmp(word, "exit") == 0)
                break;

            read(p2[0], word, 11);

            printf("%s\n", word);
        }

        exit(0);
    }
    else
    {
        pid_t pid2 = fork();
        if (pid2 == -1)
        {
            printf("Failed to create child process 2\n");
            return;
        }
        else if (pid2 == 0)
        {

            // Child 2, writes on p2[1] and reads from p1[0]
            close(p1[1]);
            close(p2[0]);

            while (1)
            {
                read(p1[0], word, 11);

                if (strcmp(word, "exit") == 0)
                    break;

                for (int i = 0; i < strlen(word); i++)
                    word[i] = toupper(word[i]);

                write(p2[1], word, 11);
            }

            exit(0);
        }
        else
        {
            // Parent Process
            close(p1[0]);
            close(p1[1]);
            close(p2[0]);
            close(p2[1]);

            wait(NULL);
            wait(NULL);
        }
    }
}