#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // For bool type
#include "../include/process.h"
#include "../include/ready_queue.h"
#include "../include/priority_ready_queue.h" // For TreeNode definition
#include "../include/sjf.h"

// Comparison function for SJF (sorts by CPU burst time)
// Lower remaining time should come first in the queue (min-heap)
int sjf_compare_process(const void *a, const void *b)
{
  ReadyQueueData *data_a = (ReadyQueueData *)a;
  ReadyQueueData *data_b = (ReadyQueueData *)b;
  Process *process_a = (Process *)data_a->process;
  Process *process_b = (Process *)data_b->process;

  if (process_a->remaining_time < process_b->remaining_time)
    return -1;
  if (process_a->remaining_time > process_b->remaining_time)
    return 1;
  // Tie-breaking by PID for deterministic behavior (optional)
  if (process_a->pid < process_b->pid)
    return -1;
  if (process_a->pid > process_b->pid)
    return 1;
  return 0;
}

ReadyQueue *new_sjf_ready_queue()
{
  printf("Creating a new SJF (Shortest Job First) ready queue using pointer-based binary heap (custom implementation)\\n");
  ReadyQueue *queue = (ReadyQueue *)malloc(sizeof(ReadyQueue));
  if (!queue)
  {
    perror("Failed to allocate memory for ReadyQueue (SJF)");
    exit(EXIT_FAILURE);
  }

  queue->root = NULL;

  queue->pop = pop_priority_ready_queue;
  queue->push = push_priority_ready_queue;
  queue->is_empty = is_empty_priority_ready_queue;
  queue->cmp = sjf_compare_process;

  return queue;
}

// Implementation of free_sjf_ready_queue
void free_sjf_ready_queue(ReadyQueue *queue)
{
  if (!queue)
    return;

  // Use the free function from priority_ready_queue
  free_priority_ready_queue(queue);
}
