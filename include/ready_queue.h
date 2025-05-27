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
  TreeNode *root;
  int size;
  ReadyQueueData *(*pop)(struct ReadyQueue *);
  void (*push)(struct ReadyQueue *, ReadyQueueData *);
  bool (*is_empty)(struct ReadyQueue *);
  int (*cmp)(const void *, const void *);
  void (*free)(struct ReadyQueue *);
} ReadyQueue;

ReadyQueueData *new_ready_queue_data(Process *process, int start_time);

#endif
