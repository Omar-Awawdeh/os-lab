#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define STATUS_READY 0
#define STATUS_BUSY 1

void execute_command(char cmd[20])
{
    pid_t pid = fork();

    if (pid == -1)
    {
        printf("Failed to create child process and execute command\n");
    }
    else if (pid == 0)
    {
        execlp(cmd, cmd, NULL);
        exit(0);
    }
    else
    {
        wait(NULL);
    }
}

struct data
{
    char cmd[20];
    int status;
} *ipc_data;

int main()
{
    key_t key = ftok("./p1.c", 1234);

    if (key == -1)
    {
        printf("Failed to get key\n");
        return 0;
    }

    // Get shared memory
    int shmid = shmget(key, sizeof(struct data), IPC_CREAT | 0666);

    if (shmid == -1)
    {
        printf("Failed to get shared memory\n");
        return 0;
    }

    // Attach shared memory
    ipc_data = (struct data *)shmat(shmid, NULL, 0);

    if (ipc_data == (void *)-1)
    {
        printf("Failed to attach shared memory\n");
        return 0;
    }

    do
    {
        sleep(1);

        // Wait for  new command
        if (ipc_data->status == STATUS_READY)
            continue;

        printf("Received command : %s\n", ipc_data->cmd);
        execute_command(ipc_data->cmd);

        // Indicate that the process is ready to take a new command
        ipc_data->status = STATUS_READY;
    } while (strcmp(ipc_data->cmd, "exit") != 0);

    // Detach shared memory
    shmctl(shmid, IPC_RMID, NULL);
    shmdt(ipc_data);

    return 0;
}
