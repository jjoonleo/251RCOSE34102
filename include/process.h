#ifndef PROCESS_H
#define PROCESS_H

#include "linked_list.h"
#include <stdlib.h>

#define MAX_IO_BURSTS_COUNT 5

typedef struct IOBurst
{
  int io_request_time;
  int io_burst_time;
  void (*print)(struct IOBurst *);
} IOBurst;

IOBurst *new_io_burst(int io_request_time, int io_burst_time);

// Function to print an IOBurst
void print_ioburst(IOBurst *io_burst);

// Function to compare two IOBurst structures by io_request_time
int compare_io_burst(const void *a, const void *b);

typedef struct Process
{
  int pid;
  int arrival_time;
  int cpu_burst;
  int priority;
  int remaining_time;
  int waiting_time;
  int turnaround_time;
  LinkedList *io_burst_list; // Linked list to store IO burst times
  void (*print)(struct Process *);
  void (*print_status)(struct Process *);
} Process;

Process *new_process(int pid);

// Function to print a Process
void print_process(Process *process);
void print_process_status(Process *process);

#endif // PROCESS_H