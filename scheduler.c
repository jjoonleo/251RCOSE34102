#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "include/process.h"

void print_io_burst(void *data)
{
  IOBurst *io_burst = (IOBurst *)data;
  printf("IO Request Time: %d, IO Burst Time: %d\n", io_burst->io_request_time, io_burst->io_burst_time);
}

int main()
{
  // int time = 0;
  //  create processes
  srand(time(NULL));
  Process *processes[5];
  printf("Creating processes:\n");
  for (int i = 0; i < 5; i++)
  {
    processes[i] = new_process(i + 1);
    printf("Process %d: Arrival Time: %d, CPU Burst: %d\n", processes[i]->pid, processes[i]->arrival_time, processes[i]->cpu_burst);
    printf("IO Bursts:\n");
    processes[i]->io_burst_list->print(processes[i]->io_burst_list, print_io_burst);
  }
}