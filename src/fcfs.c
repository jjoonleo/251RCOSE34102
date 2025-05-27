
#include <stdio.h>
#include "../include/process.h"
#include "../include/ready_queue.h"
#include "../include/fcfs.h"
#include "../include/linked_list_ready_queue.h"

ReadyQueue *new_fcfs_ready_queue()
{

  ReadyQueue *queue = (ReadyQueue *)malloc(sizeof(ReadyQueue));
  if (!queue)
  {
    perror("Failed to allocate memory for ReadyQueue");
    exit(EXIT_FAILURE);
  }
  queue->root = NULL;
  queue->pop = pop_linked_list_ready_queue;
  queue->push = push_linked_list_ready_queue;
  queue->is_empty = is_linked_list_ready_queue_empty;
  queue->free = free_linked_list_ready_queue;
  queue->cmp = NULL;
  return queue;
}
