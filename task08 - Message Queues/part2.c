#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

struct msgbuf
{
    long mtype;    /* message type, must be > 0 */
    char mtext[1]; /* message data */
};

// Parent sends type 1 and waits for type 2
void parent_process(int msgid)
{
    struct msgbuf message;

    for (int i = 1; i <= 5; i++)
    {
        printf("%d\n", i);

        // Indicate to child process that parent has printed
        message.mtype = 1;
        int msgsend = msgsnd(msgid, &message, sizeof(message.mtext), 0);

        if (msgsend == -1)
        {
            perror("msgsnd");
            exit(-1);
        }

        // Wait for child process to print
        int msgrecv = msgrcv(msgid, &message, sizeof(message), 2, 0);

        if (msgrecv == -1)
        {
            perror("msgrcv");
            exit(-1);
        }
    }
}

// Child sends type 2 and waits for type 1
void child_process_1(int msgid)
{
    struct msgbuf message;

    for (char i = 'A'; i <= 'E'; i++)
    {
        // Wait for parent process to print
        int msgrecv = msgrcv(msgid, &message, sizeof(message), 1, 0);

        if (msgrecv == -1)
        {
            perror("msgrcv");
            exit(-1);
        }

        printf("%c\n", i);

        // Indicate to parent process that the child has printed
        message.mtype = 2;
        int msgsend = msgsnd(msgid, &message, sizeof(message.mtext), 0);

        if (msgsend == -1)
        {
            perror("msgsnd");
            exit(-1);
        }
    }
}

void main()
{
    int key = ftok("./part2", 2);

    // create a message queue
    int msgid = msgget(key, IPC_CREAT | 0666);

    if (msgid == -1)
    {
        perror("msgget");
        exit(-1);
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(-1);
    }
    else if (pid == 0)
    {
        child_process_1(msgid);
    }
    else
    {
        parent_process(msgid);
    }

    // close message queue
    msgctl(key, IPC_RMID, NULL);
}
