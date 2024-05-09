#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/shm.h>

#define STATUS_READY 0
#define STATUS_BUSY 1

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
        char cmd[20];
        scanf("%s", cmd);

        if (ipc_data->status == STATUS_BUSY)
        {

            printf("Cannot take new commands until the old command is executed\n");
        }
        else
        {
            strcpy(ipc_data->cmd, cmd);
            ipc_data->status = STATUS_BUSY;
            printf("Sending [%s]\n", ipc_data->cmd);
        }
    } while (strcmp(ipc_data->cmd, "exit") != 0);

    // Detach shared memory
    shmctl(shmid, IPC_RMID, NULL);
    shmdt(ipc_data);

    return 0;
}