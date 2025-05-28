#include "linked_list.h"
#include "process.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define min(x, y) (x) < (y) ? (x) : (y)

#define max(x, y) (x) > (y) ? (x) : (y)

int compare_io_burst(const void *a, const void *b)
{
  const IOBurst *io_burst_a = (const IOBurst *)a;
  const IOBurst *io_burst_b = (const IOBurst *)b;
  return io_burst_a->io_request_time - io_burst_b->io_request_time;
}

IOBurst *new_io_burst(int io_request_time, int io_burst_time)
{
  IOBurst *io_burst = (IOBurst *)malloc(sizeof(IOBurst));
  if (!io_burst)
  {
    perror("Failed to allocate memory for IOBurst");
    exit(EXIT_FAILURE);
  }
  io_burst->io_request_time = io_request_time;
  io_burst->io_burst_time = io_burst_time;
  return io_burst;
}

void print_ioburst(IOBurst *io_burst)
{
  if (io_burst)
    printf("IO Request Time: %d, IO Burst Time: %d\n", io_burst->io_request_time, io_burst->io_burst_time);
}

Process *new_process_random(int pid)
{
  Process *p = (Process *)malloc(sizeof(Process));
  p->pid = pid;
  p->arrival_time = rand() % 10;
  p->cpu_burst = (rand() % 11) + 2;

  p->priority = rand() % 5;
  p->remaining_time = p->cpu_burst;
  p->waiting_time = 0;
  p->turnaround_time = 0;
  p->response_time = -1; // -1 indicates not yet scheduled
  p->quantum_time_level = 0;
  p->io_burst_list = new_linked_list();

  int num_io_bursts = rand() % min(MAX_IO_BURSTS_COUNT, p->cpu_burst - 1) + 1;
  for (int i = 0; i < num_io_bursts; i++)
  {
    IOBurst *io_burst = new_io_burst(rand() % (p->cpu_burst - 1) + 1, (rand() % 5) + 1);
    p->io_burst_list->insert_in_order(p->io_burst_list, (void *)io_burst, compare_io_burst);
  }
  p->io_burst_list->remove_duplicates(p->io_burst_list, compare_io_burst);

  p->print = print_process;
  p->print_status = print_process_status;
  p->copy = copy_process;
  return p;
}

Process *copy_process(Process *process)
{
  if (!process)
    return NULL;

  Process *new_process = (Process *)malloc(sizeof(Process));
  if (!new_process)
    return NULL;

  new_process->pid = process->pid;
  new_process->arrival_time = process->arrival_time;
  new_process->cpu_burst = process->cpu_burst;
  new_process->priority = process->priority;
  new_process->remaining_time = process->remaining_time;
  new_process->waiting_time = process->waiting_time;
  new_process->turnaround_time = process->turnaround_time;
  new_process->response_time = process->response_time;
  new_process->quantum_time_level = process->quantum_time_level;

  new_process->io_burst_list = new_linked_list();
  Node *current = process->io_burst_list->head;
  while (current)
  {
    IOBurst *io_burst_copy = (IOBurst *)malloc(sizeof(IOBurst));
    if (!io_burst_copy)
      return NULL;
    memcpy(io_burst_copy, current->data, sizeof(IOBurst));
    new_process->io_burst_list->append(new_process->io_burst_list, io_burst_copy);
    current = current->next;
  }

  new_process->print = print_process;
  new_process->print_status = print_process_status;
  new_process->copy = copy_process;
  return new_process;
}

void print_process(Process *process)
{
  if (!process)
    return;
  printf("Process %d: Arrival Time: %d, CPU Burst: %d, Priority: %d\n", process->pid, process->arrival_time, process->cpu_burst, process->priority);
  printf("IO Bursts:\n");
  if (process->io_burst_list && process->io_burst_list->print)
    process->io_burst_list->print(process->io_burst_list, (void (*)(void *))print_ioburst);
}

void print_process_status(Process *process)
{
  if (!process)
    return;
  printf("Process %d: Remaining Time: %d, Waiting Time: %d, Turnaround Time: %d\n", process->pid, process->remaining_time, process->waiting_time, process->turnaround_time);
}

Process *new_process(int pid, int arrival_time, int cpu_burst, int priority, LinkedList *io_burst_list)
{
  Process *p = (Process *)malloc(sizeof(Process));
  if (!p)
  {
    perror("Error: Failed to allocate memory for Process");
    exit(EXIT_FAILURE);
  }
  if (io_burst_list == NULL)
  {
    perror("Error: IO Burst List cannot be NULL");
  }

  p->pid = pid;
  p->arrival_time = arrival_time;
  p->cpu_burst = cpu_burst;
  p->priority = priority;
  p->remaining_time = p->cpu_burst;
  p->waiting_time = 0;
  p->turnaround_time = 0;
  p->response_time = -1; // -1 indicates not yet scheduled
  p->quantum_time_level = 0;
  p->io_burst_list = io_burst_list;

  p->print = print_process;
  p->print_status = print_process_status;
  p->copy = copy_process;
  return p;
}

Process **create_processes_keyboard(int *num_processes)
{

  do
  {
    printf("Enter number of processes: ");
    while (scanf("%d", num_processes) != 1)
    {
      printf("Invalid input! Please enter a number.\n");
      while (getchar() != '\n')
        ;
      printf("Enter number of processes: ");
    }
    if (*num_processes <= 0)
    {
      printf("Error: Number of processes must be greater than 0\n");
    }
  } while (*num_processes <= 0);

  Process **processes = (Process **)malloc(sizeof(Process *) * (*num_processes));
  if (!processes)
  {
    perror("Error: Memory allocation failed for processes array");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < *num_processes; i++)
  {
    int arrival_time, cpu_burst, priority;
    printf("\nProcess %d:\n", i + 1);

    do
    {
      printf("Enter arrival time: ");
      while (scanf("%d", &arrival_time) != 1)
      {
        printf("Invalid input! Please enter a number.\n");
        while (getchar() != '\n')
          ;
        printf("Enter arrival time: ");
      }
      if (arrival_time < 0)
      {
        printf("Error: Arrival time cannot be negative\n");
      }
    } while (arrival_time < 0);

    do
    {
      printf("Enter CPU burst time: ");
      while (scanf("%d", &cpu_burst) != 1)
      {
        printf("Invalid input! Please enter a number.\n");
        while (getchar() != '\n')
          ;
        printf("Enter CPU burst time: ");
      }
      if (cpu_burst <= 0)
      {
        printf("Error: CPU burst time must be greater than 0\n");
      }
    } while (cpu_burst <= 0);

    do
    {
      printf("Enter priority (0-%d, lower is higher priority): ", *num_processes - 1);
      while (scanf("%d", &priority) != 1)
      {
        printf("Invalid input! Please enter a number.\n");
        while (getchar() != '\n')
          ;
        printf("Enter priority (0-4, lower is higher priority): ");
      }
      if (priority < 0 || priority > *num_processes - 1)
      {
        printf("Error: Priority must be between 0 and %d\n", *num_processes - 1);
      }
    } while (priority < 0 || priority > *num_processes - 1);

    LinkedList *io_burst_list = new_linked_list();
    int num_io_bursts;
    int max_io_bursts = min(MAX_IO_BURSTS_COUNT, cpu_burst - 1);

    do
    {
      printf("Enter number of IO bursts (0-%d): ", max_io_bursts);
      while (scanf("%d", &num_io_bursts) != 1)
      {
        printf("Invalid input! Please enter a number.\n");
        while (getchar() != '\n')
          ;
        printf("Enter number of IO bursts (0-%d): ", max_io_bursts);
      }
      if (num_io_bursts < 0 || num_io_bursts > max_io_bursts)
      {
        printf("Error: Number of IO bursts must be between 0 and %d\n", max_io_bursts);
      }
    } while (num_io_bursts < 0 || num_io_bursts > max_io_bursts);

    if (num_io_bursts > 0)
    {
      for (int j = 0; j < num_io_bursts; j++)
      {
        int io_request_time, io_burst_time;

        do
        {
          printf("  IO Burst %d - Request time (1-%d): ", j + 1, cpu_burst - 1);
          while (scanf("%d", &io_request_time) != 1)
          {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n')
              ;
            printf("  IO Burst %d - Request time (1-%d): ", j + 1, cpu_burst - 1);
          }
          if (io_request_time <= 0 || io_request_time >= cpu_burst)
          {
            printf("Error: IO request time must be between 1 and %d\n", cpu_burst - 1);
          }
        } while (io_request_time <= 0 || io_request_time >= cpu_burst);

        do
        {
          printf("  IO Burst %d - Burst duration: ", j + 1);
          while (scanf("%d", &io_burst_time) != 1)
          {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n')
              ;
            printf("  IO Burst %d - Burst duration: ", j + 1);
          }
          if (io_burst_time <= 0)
          {
            printf("Error: IO burst duration must be greater than 0\n");
          }
        } while (io_burst_time <= 0);

        IOBurst *io_burst = new_io_burst(io_request_time, io_burst_time);
        io_burst_list->insert_in_order(io_burst_list, (void *)io_burst, compare_io_burst);
      }
      io_burst_list->remove_duplicates(io_burst_list, compare_io_burst);
    }

    processes[i] = new_process(i + 1, arrival_time, cpu_burst, priority, io_burst_list);
  }

  return processes;
}

Process **create_processes_from_file(const char *filename, int *num_processes)
{
  FILE *file = fopen(filename, "r");
  if (!file)
  {
    printf("Error: Could not open file '%s'\n", filename);
    *num_processes = 0;
    return NULL;
  }

  if (fscanf(file, "%d", num_processes) != 1 || *num_processes <= 0)
  {
    printf("Error: Invalid number of processes in file\n");
    fclose(file);
    *num_processes = 0;
    return NULL;
  }

  Process **processes = (Process **)malloc(sizeof(Process *) * (*num_processes));
  if (!processes)
  {
    perror("Failed to allocate memory for processes array");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < *num_processes; i++)
  {
    int arrival_time, cpu_burst, priority, num_io_bursts;

    if (fscanf(file, "%d %d %d %d", &arrival_time, &cpu_burst, &priority, &num_io_bursts) != 4)
    {
      printf("Error: Invalid process data in file for process %d\n", i + 1);
      fclose(file);

      for (int j = 0; j < i; j++)
      {
        free(processes[j]);
      }
      free(processes);
      *num_processes = 0;
      return NULL;
    }

    if (cpu_burst <= 0)
      cpu_burst = 1;
    if (priority < 0 || priority > 4)
      priority = 2;

    LinkedList *io_burst_list = new_linked_list();
    int max_io_bursts = min(num_io_bursts, min(MAX_IO_BURSTS_COUNT, cpu_burst - 1));
    for (int j = 0; j < max_io_bursts; j++)
    {
      int io_request_time, io_burst_time;
      if (fscanf(file, "%d %d", &io_request_time, &io_burst_time) == 2)
      {
        if (io_request_time > 0 && io_request_time < cpu_burst && io_burst_time > 0)
        {
          IOBurst *io_burst = new_io_burst(io_request_time, io_burst_time);
          io_burst_list->insert_in_order(io_burst_list, (void *)io_burst, compare_io_burst);
        }
      }
      else
      {
        break;
      }
    }
    io_burst_list->remove_duplicates(io_burst_list, compare_io_burst);
    processes[i] = new_process(i + 1, arrival_time, cpu_burst, priority, io_burst_list);
  }

  fclose(file);
  return processes;
}

Process **create_processes_random(int num_processes)
{
  if (num_processes <= 0)
  {
    return NULL;
  }

  Process **processes = (Process **)malloc(sizeof(Process *) * num_processes);
  if (!processes)
  {
    perror("Failed to allocate memory for processes array");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < num_processes; i++)
  {
    processes[i] = new_process_random(i + 1);
  }

  return processes;
}