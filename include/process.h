#ifndef PROCESS_H
#define PROCESS_H

#include "linked_list.h"
#include <stdlib.h>

#define MAX_IO_BURSTS_COUNT 2

typedef struct IOBurst
{
  int io_request_time;
  int io_burst_time;
  void (*print)(struct IOBurst *);
} IOBurst;

IOBurst *new_io_burst(int io_request_time, int io_burst_time);

void print_ioburst(IOBurst *io_burst);

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
  int queue_level;
  LinkedList *io_burst_list;
  void (*print)(struct Process *);
  void (*print_status)(struct Process *);
  struct Process *(*copy)(struct Process *);
} Process;

Process *new_process_random(int pid);
Process *new_process_manual(int pid, int arrival_time, int cpu_burst, int priority, LinkedList *io_burst_list);
Process *copy_process(Process *process);

Process **create_processes_keyboard(int *num_processes);
Process **create_processes_from_file(const char *filename, int *num_processes);
Process **create_processes_random(int num_processes);

void print_process(Process *process);
void print_process_status(Process *process);

#endif