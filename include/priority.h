#ifndef PRIORITY_H
#define PRIORITY_H

#include "process.h"
#include "ready_queue.h"
#include "priority_queue.h"

int priority_compare_process(const void *a, const void *b);
ReadyQueue *new_priority_ready_queue();

#endif
