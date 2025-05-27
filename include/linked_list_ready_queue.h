#ifndef linked_list_ready_queue_H
#define linked_list_ready_queue_H

#include <stdbool.h>
#include "ready_queue.h"

ReadyQueueData *pop_linked_list_ready_queue(ReadyQueue *queue);
void push_linked_list_ready_queue(ReadyQueue *queue, ReadyQueueData *data);
bool is_linked_list_ready_queue_empty(ReadyQueue *queue);
void free_linked_list_ready_queue(ReadyQueue *queue);

#endif
