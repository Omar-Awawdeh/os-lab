#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Process
{
    int id;
    int arrival_time;
    int lifetime;
    int memory;
    int allocated;
    int base;
};

struct MemoryHole
{
    int base;
    int limit;
    struct MemoryHole *next;
};

void generate_processes(struct Process processes[], int n, int total_memory)
{
    printf("Generating random timeline\n");
    printf("PROCESS\t| ARRIVAL TIME\t| LIFETIME\t| MEMORY REQUIREMENTS\n");

    for (int i = 0; i < n; i++)
    {
        processes[i].arrival_time = rand() % 21;
        processes[i].lifetime = 10 + rand() % 11;
        // Add 1 to prevent processes that don't use any memory
        processes[i].memory = (rand() % (total_memory / n)) + 1;
        processes[i].allocated = 0;
        printf("P%d\t  %d\t\t %d\t\t %d\n", i, processes[i].arrival_time, processes[i].lifetime, processes[i].memory);
    }
}

void print_memory_holes(struct MemoryHole *holes)
{
    printf("List of memory holes = [ ");
    struct MemoryHole *current = holes;
    while (current != NULL)
    {
        printf("%d", current->limit);
        if (current->next != NULL)
        {
            printf(" --> ");
        }
        current = current->next;
    }
    printf(" ]\n\n");
}

void allocate_memory(struct Process processes[], int i, struct MemoryHole *chosen_hole)
{
    processes[i].allocated = 1;
    processes[i].base = chosen_hole->base;
    chosen_hole->base += processes[i].memory;
    chosen_hole->limit -= processes[i].memory;

    // If hole is full, move pointer one step forward and free the memory
    if (chosen_hole->limit == 0)
    {
        struct MemoryHole *temp = chosen_hole->next;
        *chosen_hole = *chosen_hole->next;
        free(temp);
    }

    printf("Allocating memory to P%d at time %d: [Base = %d, Limit = %d]\n", i, processes[i].arrival_time, processes[i].base, processes[i].memory);
    print_memory_holes(chosen_hole);
}

void deallocate_memory(struct Process processes[], int i, int n, struct MemoryHole **head, int time)
{
    printf("Deallocating memory of P%d at time %d\n", i, time);
    struct MemoryHole *new_hole = (struct MemoryHole *)malloc(sizeof(struct MemoryHole));
    new_hole->base = processes[i].base;
    new_hole->limit = processes[i].memory;
    new_hole->next = NULL;

    if (*head == NULL)
    {
        *head = new_hole;
    }
    else
    {
        struct MemoryHole *current = *head;
        struct MemoryHole *prev = NULL;
        while (current != NULL && current->base < new_hole->base)
        {
            prev = current;
            current = current->next;
        }
        new_hole->next = current;
        if (prev == NULL)
        {
            *head = new_hole;
        }
        else
        {
            prev->next = new_hole;
        }

        // Merge adjacent memory holes
        if (prev != NULL && prev->base + prev->limit == new_hole->base)
        {
            prev->limit += new_hole->limit;
            prev->next = new_hole->next;
            free(new_hole);
            new_hole = prev;
        }
        if (new_hole->next != NULL && new_hole->base + new_hole->limit == new_hole->next->base)
        {
            struct MemoryHole *temp = new_hole->next;
            new_hole->limit += new_hole->next->limit;
            new_hole->next = new_hole->next->next;
            free(temp);
        }
    }
    print_memory_holes(*head);
}
void first_fit_allocation(struct Process processes[], int i, int n, struct MemoryHole *head)
{
    struct MemoryHole *first_hole = NULL;
    struct MemoryHole *current = head;
    while (current != NULL)
    {
        if (current->limit >= processes[i].memory)
        {
            first_hole = current;
            break;
        }
        current = current->next;
    }
    if (first_hole != NULL)
    {
        allocate_memory(processes, i, first_hole);
    }
    else
    {
        printf("No hole big enough to accommodate P%d at time %d\n", i, time);
    }
}

void best_fit_allocation(struct Process processes[], int i, int n, struct MemoryHole *head)
{

    struct MemoryHole *best_hole = NULL;
    struct MemoryHole *current = head;

    while (current != NULL)
    {
        if (current->limit >= processes[i].memory)
        {
            if (best_hole == NULL || current->limit < best_hole->limit)
            {
                best_hole = current;
            }
        }
        current = current->next;
    }

    // Handle no best hole found
    if (best_hole != NULL)
        allocate_memory(processes, i, best_hole);
    else
        printf("No hole big enough to accommodate P%d at time %d\n", i, time);
}

void worst_fit_allocation(struct Process processes[], int i, int n, struct MemoryHole *head)
{

    struct MemoryHole *best_hole = NULL;
    struct MemoryHole *current = head;

    while (current != NULL)
    {
        if (current->limit >= processes[i].memory)
        {
            if (best_hole == NULL || current->limit > best_hole->limit)
            {
                best_hole = current;
            }
        }
        current = current->next;
    }

    // Handle no best hole found
    if (best_hole != NULL)
        allocate_memory(processes, i, best_hole);
    else
        printf("No hole big enough to accommodate P%d at time %d\n", i, time);
}

int main()
{
    srand(time(NULL));
    int total_memory, n, choice;

    printf("Enter the memory size: ");
    scanf("%d", &total_memory);
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process processes[n];
    generate_processes(processes, n, total_memory);

    printf("Choose algorithm (BEST[1], WORST[2], FIRST[3]): ");
    scanf("%d", &choice);

    struct MemoryHole *head = (struct MemoryHole *)malloc(sizeof(struct MemoryHole));
    head->base = 0;
    head->limit = total_memory;
    head->next = NULL;

    for (int time = 0; time <= 40; time++)
    {
        for (int i = 0; i < n; i++)
        {
            if (time == processes[i].arrival_time && processes[i].allocated == 0)
            {
                switch (choice)
                {
                case 1:
                    best_fit_allocation(processes, i, n, head);
                    break;
                case 2:
                    worst_fit_allocation(processes, i, n, head);
                    break;
                case 3:
                    first_fit_allocation(processes, i, n, head);
                    break;
                }
            }
            if (processes[i].allocated && time == processes[i].arrival_time + processes[i].lifetime)
            {
                deallocate_memory(processes, i, n, &head, time);
            }
        }
    }

    return 0;
}