#ifndef PROCESS_H
#define PROCESS_H

#include "linked_list.h"
#include <stdlib.h>

#define MAX_IO_BURSTS_COUNT 5

typedef struct
{
  int io_request_time;
  int io_burst_time;
} IOBurst;

IOBurst *new_io_burst(int io_request_time, int io_burst_time);

// Function to compare two IOBurst structures by io_request_time
int compare_io_burst(const void *a, const void *b);

typedef struct
{
  int pid;
  int arrival_time;
  int cpu_burst;
  int priority;
  int remaining_time;
  int waiting_time;
  int turnaround_time;
  LinkedList *io_burst_list; // Linked list to store IO burst times
} Process;

Process *new_process(int pid);

#endif // PROCESS_H