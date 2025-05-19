#ifndef FCFS_H
#define FCFS_H

#include "process.h"
#include "ready_queue.h"

ReadyQueueData *pop(ReadyQueue *queue);
void push(ReadyQueue *queue, ReadyQueueData *process);
bool is_ready_queue_empty(ReadyQueue *queue);
void print_ready_queue(ReadyQueue *queue);
ReadyQueue *new_fcfs_ready_queue();
void test(ReadyQueue *queue);

#endif // FCFS_H
