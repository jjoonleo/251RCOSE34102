#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/process.h"
#include "../include/ready_queue.h"
#include "../include/priority_ready_queue.h"
#include "../include/cfs.h"

int get_process_weight(int priority)
{

  int weights[] = {1024, 820, 655, 526, 421};
  if (priority < 0 || priority > 4)
  {
    return 1024;
  }
  return weights[priority];
}

int calculate_cfs_timeslice(Process *process, int num_ready_processes)
{
  if (num_ready_processes <= 0)
  {
    return CFS_TARGET_LATENCY;
  }

  int base_slice = CFS_TARGET_LATENCY / num_ready_processes;

  if (base_slice < CFS_MIN_GRANULARITY)
  {
    base_slice = CFS_MIN_GRANULARITY;
  }

  int weight = get_process_weight(process->priority);
  int default_weight = get_process_weight(2);

  int weighted_slice = (base_slice * weight) / default_weight;

  return (weighted_slice < CFS_MIN_GRANULARITY) ? CFS_MIN_GRANULARITY : weighted_slice;
}

int cfs_compare_process(const void *a, const void *b)
{
  ReadyQueueData *data_a = (ReadyQueueData *)a;
  ReadyQueueData *data_b = (ReadyQueueData *)b;
  Process *process_a = (Process *)data_a->process;
  Process *process_b = (Process *)data_b->process;

  if (process_a->vruntime < process_b->vruntime)
    return -1;
  if (process_a->vruntime > process_b->vruntime)
    return 1;

  if (process_a->pid < process_b->pid)
    return -1;
  if (process_a->pid > process_b->pid)
    return 1;

  return 0;
}

ReadyQueue *new_cfs_ready_queue()
{
  ReadyQueue *queue = (ReadyQueue *)malloc(sizeof(ReadyQueue));
  if (!queue)
  {
    perror("Failed to allocate memory for ReadyQueue (CFS)");
    exit(EXIT_FAILURE);
  }

  queue->root = NULL;
  queue->size = 0;

  queue->pop = pop_priority_ready_queue;
  queue->push = push_priority_ready_queue;
  queue->is_empty = is_empty_priority_ready_queue;
  queue->cmp = cfs_compare_process;
  queue->free = free_priority_ready_queue;

  return queue;
}