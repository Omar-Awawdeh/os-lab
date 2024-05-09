#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void *count_uppercase(void *param)
{
    char *input = param;
    int *output = malloc(sizeof(int));
    *output = 0;

    for (int i = 0; i < strlen(input); i++)
        if (isupper(input[i]))
            *output = *output + 1;

    return output;
}

void *count_lowercase(void *param)
{
    char *input = param;
    int *output = malloc(sizeof(int));
    *output = 0;

    for (int i = 0; i < strlen(input); i++)
        if (islower(input[i]))
            *output = *output + 1;

    return output;
}

void main()
{
    char input[51];

    printf("Please enter a string: ");
    fgets(input, 51, stdin);

    pthread_t tid1, tid2;
    int *uppercase_count, *lowercase_count;

    pthread_create(&tid1, NULL, count_uppercase, &input);
    pthread_create(&tid2, NULL, count_lowercase, &input);

    pthread_join(tid1, (void **)&uppercase_count);
    pthread_join(tid2, (void **)&lowercase_count);

    printf("Lowercase letter count = %d\nUppercase letter count = %d\n", *lowercase_count, *uppercase_count);

    free(uppercase_count);
    free(lowercase_count);
}
