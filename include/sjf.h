#ifndef SJF_H
#define SJF_H

#include "process.h"
#include "ready_queue.h"
#include "priority_queue.h"

// Comparison function for SJF (compares CPU burst times)
int sjf_compare_process(const void *a, const void *b);

ReadyQueueData *sjf_pop(ReadyQueue *queue);
void sjf_push(ReadyQueue *queue, ReadyQueueData *process);
bool sjf_is_ready_queue_empty(ReadyQueue *queue);
void sjf_print_ready_queue(ReadyQueue *queue);
ReadyQueue *new_sjf_ready_queue();
void sjf_test(ReadyQueue *queue);
void free_sjf_ready_queue(ReadyQueue *queue);

#endif // SJF_H
