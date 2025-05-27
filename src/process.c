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

Process *new_process(int pid)
{
  Process *p = (Process *)malloc(sizeof(Process));
  p->pid = pid;
  p->arrival_time = rand() % 10;
  p->cpu_burst = (rand() % 11) + 2;

  p->priority = rand() % 5;
  p->remaining_time = p->cpu_burst;
  p->waiting_time = 0;
  p->turnaround_time = 0;
  p->vruntime = 0;
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
  new_process->vruntime = process->vruntime;

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