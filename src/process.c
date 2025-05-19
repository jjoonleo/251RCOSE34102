#include "linked_list.h"
#include "process.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#define min(x, y) (x) < (y) ? (x) : (y)

#define max(x, y) (x) > (y) ? (x) : (y)

// Function to compare two IOBurst structures by io_request_time
int compare_io_burst(const void *a, const void *b)
{
  const IOBurst *io_burst_a = (const IOBurst *)a;
  const IOBurst *io_burst_b = (const IOBurst *)b;
  return io_burst_a->io_request_time - io_burst_b->io_request_time;
}

// Function to create a new IOBurst structure
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

// Function to print an IOBurst
void print_ioburst(IOBurst *io_burst)
{
  if (io_burst)
    printf("IO Request Time: %d, IO Burst Time: %d\n", io_burst->io_request_time, io_burst->io_burst_time);
}

Process *new_process(int pid)
{
  Process *p = (Process *)malloc(sizeof(Process));
  p->pid = pid;
  p->arrival_time = rand() % 10;    // Random arrival time
  p->cpu_burst = (rand() % 11) + 2; // Random CPU burst time

  p->priority = rand() % 5; // Random priority
  p->remaining_time = p->cpu_burst;
  p->waiting_time = 0;
  p->turnaround_time = 0;
  p->io_burst_list = createLinkedList(); // Initialize the IO burst linked list

  // Add random IO burst times in ascending order
  int num_io_bursts = rand() % min(MAX_IO_BURSTS_COUNT, p->cpu_burst - 1) + 1; // Random number of IO bursts (0 to 4)
  for (int i = 0; i < num_io_bursts; i++)
  {
    IOBurst *io_burst = new_io_burst(rand() % (p->cpu_burst - 1) + 1, (rand() % 5) + 1); // Random IO request time and burst time
    p->io_burst_list->insertInOrder(p->io_burst_list, (void *)io_burst, compare_io_burst);
  }
  p->io_burst_list->removeDuplicates(p->io_burst_list, compare_io_burst); // Remove duplicates if any

  p->print = print_process;               // Assign the print function
  p->print_status = print_process_status; // Assign the print status function
  return p;
}

// Function to print a Process
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