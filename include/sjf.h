#ifndef SJF_H
#define SJF_H

#include "process.h"
#include "ready_queue.h"
#include "priority_queue.h"

int sjf_compare_process(const void *a, const void *b);
ReadyQueue *new_sjf_ready_queue();

#endif
