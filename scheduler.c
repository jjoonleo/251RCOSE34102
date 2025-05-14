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
  }

  // Find min and max arrival times
  int min_arrival = processes[0]->arrival_time;
  int max_arrival = processes[0]->arrival_time;
  for (int i = 1; i < 5; i++)
  {
    if (processes[i]->arrival_time < min_arrival)
      min_arrival = processes[i]->arrival_time;
    if (processes[i]->arrival_time > max_arrival)
      max_arrival = processes[i]->arrival_time;
  }
  int range = max_arrival - min_arrival + 1;

  // Allocate 2D array for process pointers
  Process **arrival_buckets[range];
  int bucket_counts[range];
  for (int i = 0; i < range; i++)
  {
    arrival_buckets[i] = malloc(5 * sizeof(Process *)); // max 5 processes per bucket
    bucket_counts[i] = 0;
  }

  // Place processes in buckets by arrival time
  for (int i = 0; i < 5; i++)
  {
    int idx = processes[i]->arrival_time - min_arrival;
    arrival_buckets[idx][bucket_counts[idx]++] = processes[i];
  }

  // Print 2D array of process IDs ordered by arrival time
  printf("\nProcesses grouped by arrival time (ordered):\n");
  for (int i = 0; i < range; i++)
  {
    if (bucket_counts[i] > 0)
    {
      printf("\n---------------------\n\n");
      for (int j = 0; j < bucket_counts[i]; j++)
      {
        arrival_buckets[i][j]->print(arrival_buckets[i][j]);
      }
    }
  }
  printf("\n");

  // Free allocated memory for buckets
  for (int i = 0; i < range; i++)
  {
    free(arrival_buckets[i]);
  }
}