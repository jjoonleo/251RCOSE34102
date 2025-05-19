#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include "process.h"
#include "linked_list.h"
#include "priority_queue.h"
#include <stdbool.h>

typedef struct ReadyQueueData
{
  Process *process;
  int start_time;
} ReadyQueueData;

typedef struct ReadyQueue
{
  TreeNode *head;
  void *private_data; // For storing priority queue or other data structures
  ReadyQueueData *(*pop)(struct ReadyQueue *);
  void (*push)(struct ReadyQueue *, ReadyQueueData *);
  bool (*is_empty)(struct ReadyQueue *);
  void (*print)(struct ReadyQueue *);
  void (*test)(struct ReadyQueue *);
} ReadyQueue;

ReadyQueueData *new_ready_queue_data(Process *process, int start_time);

#endif // READY_QUEUE_H
