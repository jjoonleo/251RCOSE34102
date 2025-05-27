#ifndef MULTI_LEVEL_READY_QUEUE_H
#define MULTI_LEVEL_READY_QUEUE_H

#include "ready_queue.h"
#include "process.h"

#define MLFQ_Q0_QUANTUM 2
#define MLFQ_Q1_QUANTUM 4
#define MLFQ_Q2_QUANTUM -1

ReadyQueue *new_multi_level_ready_queue(LinkedList *ready_queues);
ReadyQueueData *pop_multi_level_ready_queue(ReadyQueue *queue);
void push_multi_level_ready_queue(ReadyQueue *queue, ReadyQueueData *data);
bool is_multi_level_ready_queue_empty(ReadyQueue *queue);
void free_multi_level_ready_queue(ReadyQueue *queue);

void mlfq_demote_process(Process *process);
int mlfq_get_time_quantum(int queue_level);

#endif