#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/process.h"
#include "../include/ready_queue.h"
#include "../include/priority_ready_queue.h"
#include "../include/sjf.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int sjf_compare_process(const void *a, const void *b)
{
  ReadyQueueData *data_a = (ReadyQueueData *)a;
  ReadyQueueData *data_b = (ReadyQueueData *)b;
  Process *process_a = (Process *)data_a->process;
  Process *process_b = (Process *)data_b->process;

  int remaining_time_a = process_a->remaining_time;
  IOBurst *io_a = process_a->io_burst_list->get_first(process_a->io_burst_list);
  int current_time_a = process_a->cpu_burst - process_a->remaining_time;
  if (io_a != NULL)
  {
    remaining_time_a = MIN(remaining_time_a, io_a->io_request_time - current_time_a);
  }

  int remaining_time_b = process_b->remaining_time;
  IOBurst *io_b = process_b->io_burst_list->get_first(process_b->io_burst_list);
  int current_time_b = process_b->cpu_burst - process_b->remaining_time;
  if (io_b != NULL)
  {
    remaining_time_b = MIN(remaining_time_b, io_b->io_request_time - current_time_b);
  }

  if (remaining_time_a < remaining_time_b)
    return -1;
  if (remaining_time_a > remaining_time_b)
    return 1;

  if (process_a->pid < process_b->pid)
    return -1;
  if (process_a->pid > process_b->pid)
    return 1;
  return 0;
}

ReadyQueue *new_sjf_ready_queue()
{
  ReadyQueue *queue = (ReadyQueue *)malloc(sizeof(ReadyQueue));
  if (!queue)
  {
    perror("Failed to allocate memory for ReadyQueue (SJF)");
    exit(EXIT_FAILURE);
  }

  queue->root = NULL;
  queue->size = 0;

  queue->pop = pop_priority_ready_queue;
  queue->push = push_priority_ready_queue;
  queue->is_empty = is_empty_priority_ready_queue;
  queue->cmp = sjf_compare_process;
  queue->free = free_priority_ready_queue;

  return queue;
}