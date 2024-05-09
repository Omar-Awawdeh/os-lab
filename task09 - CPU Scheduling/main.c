#include <stdio.h>

void copy_array(int n, int src[], int copy[])
{
    for (int i = 0; i < n; i++)
        copy[i] = src[i];
}

void bubble_sort(int n, int arr[])
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n - 1; j++)
            if (arr[j] > arr[i])
            {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
}

void output(int n, int bursts[], int waiting[], int turn_around[])
{
    int waiting_sum = 0, turn_around_sum = 0;

    printf("\n| Process\t| Burst\t\t| Waiting\t| Turnaround\n");

    for (int i = 0; i < n; i++)
    {
        printf("| P%d\t\t| %d\t\t| %d\t\t| %d\n", i + 1, bursts[i], waiting[i], turn_around[i]);

        waiting_sum += waiting[i];
        turn_around_sum += turn_around[i];
    }

    printf("Average Waiting Time = %f\n", (float)waiting_sum / n);
    printf("Average Turnaround Time = %f\n", (float)turn_around_sum / n);
}

void first_come_first_serve(int n, int bursts[])
{
    int waiting[n], turnaround[n];

    waiting[0] = 0;
    turnaround[0] = bursts[0];

    for (int i = 1; i < n; i++)
    {
        waiting[i] = waiting[i - 1] + bursts[i - 1];
        turnaround[i] = waiting[i] + bursts[i];
    }

    output(n, bursts, waiting, turnaround);
}

void shortest_job_first(int n, int burst[])
{
    int burst_copy[n];
    copy_array(n, burst, burst_copy);
    bubble_sort(n, burst_copy);
    first_come_first_serve(n, burst_copy);
}

void round_robin(int n, int bursts[], int q)
{
    int waiting[n], turnaround[n], bursts_copy[n];

    copy_array(n, bursts, bursts_copy);

    for (int i = 0; i < n; i++)
    {
        waiting[i] = 0;
        turnaround[i] = 0;
    }

    int turn_count = 0, turn = 0, time_passed = 0;
    // Exit the loop when we did a full loop without finding any process with unfinished burst
    while (turn_count != n)
    {
        turn_count++;

        if (bursts_copy[turn] > 0)
        {
            turn_count = 0;
            if (bursts_copy[turn] > q)
            {
                time_passed += q;
                bursts_copy[turn] -= q;
            }
            else
            {
                time_passed += bursts_copy[turn];
                bursts_copy[turn] = 0;
                waiting[turn] = time_passed - bursts[turn];
                turnaround[turn] = time_passed;
            }
        }

        // Circular loop over the processes
        turn = (turn + 1) % n;
    }

    output(n, bursts, waiting, turnaround);
}

int main()
{
    // Get process information from user
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    int bursts[n];
    for (int i = 0; i < n; i++)
    {
        printf("Enter the burst time of process #%d: ", i + 1);
        scanf("%d", &bursts[i]);
    }

    // Display menu to choose scheduling algorithm
    while (1)
    {
        int input;
        printf("\n1. FCFS\n2. RR\n3. SJF\n4. Exit\nSelect the scheduling algorithm to use: ");
        scanf("%d", &input);

        switch (input)
        {
        case 1:
            first_come_first_serve(n, bursts);
            break;
        case 2:
            int q;
            printf("Enter quantum to use: ");
            scanf("%d", &q);
            round_robin(n, bursts, q);
            break;
        case 3:
            shortest_job_first(n, bursts);
            break;
        case 4:
            return 0;
        }
    }

    return 0;
}