#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define MESSAGE_SIZE 51

struct msgbuf
{
    long mtype;               /* message type, must be > 0 */
    char mtext[MESSAGE_SIZE]; /* message data */
};

void parent_process(int msgid)
{
    struct msgbuf message;
    char input[MESSAGE_SIZE];

    while (1)
    {
        // Get input string from stdin
        fgets(input, sizeof(input), stdin);

        // Send message to child 1
        strcpy(message.mtext, input);
        message.mtype = 1;

        int msgsend = msgsnd(msgid, &message, sizeof(message.mtext), 0);

        if (msgsend == -1)
        {
            perror("msgsnd");
            exit(-1);
        }
        if (strcmp(input, "exit\n") == 0)
        {
            printf("Parent exiting\n");
            exit(0);
        }
    }
}

void child_process_1(int msgid)
{
    struct msgbuf message;

    while (1)
    {
        // Get message from parent
        int msgrecv = msgrcv(msgid, &message, sizeof(message), 1, 0);

        if (msgrecv == -1)
        {
            perror("msgrcv");
            exit(-1);
        }

        // Convert message text to lowercase
        for (int i = 0; i < strlen(message.mtext); i++)
        {
            message.mtext[i] = tolower(message.mtext[i]);
        }

        // Send message to child 3
        message.mtype = 2;
        int msgsend = msgsnd(msgid, &message, sizeof(message.mtext), 0);

        if (msgsend == -1)
        {
            perror("msgsnd");
            exit(-1);
        }

        if (strcmp(message.mtext, "exit\n") == 0)
        {
            printf("Child 1 exiting\n");
            exit(0);
        }
    }
}

void child_process_2(int msgid)
{
    struct msgbuf message;

    while (1)
    {
        // Receive message from child 1
        int msgrecv = msgrcv(msgid, &message, sizeof(message), 2, 0);

        if (msgrecv == -1)
        {
            perror("msgrcv");

            exit(-1);
        }

        if (strcmp(message.mtext, "exit\n") == 0)
        {
            printf("Child 2 exiting\n");
            exit(0);
        }

        printf("%s", message.mtext);
    }
}

void main()
{
    int key = ftok("./part3", 2);

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
        exit(0);
    }

    pid_t pid2 = fork();

    if (pid2 == -1)
    {
        perror("fork");
        exit(-1);
    }
    else if (pid2 == 0)
    {
        child_process_2(msgid);
        exit(0);
    }

    parent_process(msgid);
}
