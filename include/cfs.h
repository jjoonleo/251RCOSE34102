#ifndef CFS_H
#define CFS_H

#include "ready_queue.h"

#define CFS_TARGET_LATENCY 6
#define CFS_MIN_GRANULARITY 1

ReadyQueue *new_cfs_ready_queue();

int cfs_compare_process(const void *a, const void *b);

int calculate_cfs_timeslice(Process *process, int num_ready_processes);

int get_process_weight(int priority);

#endif