#ifndef priority_ready_queue_H
#define priority_ready_queue_H

#include <stdbool.h>
#include "ready_queue.h"

ReadyQueue *new_priority_ready_queue(int (*cmp)(const void *, const void *));
ReadyQueueData *pop_priority_ready_queue(ReadyQueue *queue);
void push_priority_ready_queue(ReadyQueue *queue, ReadyQueueData *data);
bool is_empty_priority_ready_queue(ReadyQueue *queue);
void free_priority_ready_queue(ReadyQueue *queue);

#endif
