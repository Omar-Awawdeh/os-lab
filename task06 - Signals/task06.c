#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int count = 1;
int paused = 0;

void handler(int signo)
{
    if (signo == SIGINT)
        paused = !paused;

    if (signo == SIGHUP)
        count = 1;
}

void main()
{
    signal(SIGALRM, SIG_IGN);

    signal(SIGHUP, handler);
    signal(SIGINT, handler);

    printf("My PID is %d\n", getpid());

    while (1)
    {
        if (paused)
            continue;

        printf("%d\n", count);

        if (count == 15)
            count = 1;
        else
            count++;

        sleep(1);
    }
}