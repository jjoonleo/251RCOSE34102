#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/process.h"
#include "../include/ready_queue.h"
#include "../include/priority_ready_queue.h"
#include "../include/priority.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int priority_compare_process(const void *a, const void *b)
{
  ReadyQueueData *data_a = (ReadyQueueData *)a;
  ReadyQueueData *data_b = (ReadyQueueData *)b;
  Process *process_a = (Process *)data_a->process;
  Process *process_b = (Process *)data_b->process;

  if (process_a->priority < process_b->priority)
    return -1;
  if (process_a->priority > process_b->priority)
    return 1;
  if (process_a->pid < process_b->pid)
    return -1;
  if (process_a->pid > process_b->pid)
    return 1;
  return 0;
}

ReadyQueue *new_priority_priority_ready_queue()
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
  queue->cmp = priority_compare_process;
  queue->free = free_priority_ready_queue;

  return queue;
}