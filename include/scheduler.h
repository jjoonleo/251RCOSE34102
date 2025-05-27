#include "priority_queue.h"
#include "ready_queue.h"
#include "gantt_chart.h"

typedef struct Schedular
{
  int total_turnaround_time;
  int total_waiting_time;
  int time;
  int time_quantum;
  PriorityQueue *event_queue;
  ReadyQueue *ready_queue;
  GanttChart *gantt_chart;
  bool is_preemptive;
  void (*schedule)(struct Schedular *schedular);
  void (*free)(struct Schedular *schedular);
} Schedular;

Schedular *new_schedular(Process *processes[], int process_count, ReadyQueue *ready_queue, int is_preemptive);
Schedular *new_schedular_with_quantum(Process *processes[], int process_count, ReadyQueue *ready_queue, int is_preemptive, int time_quantum);
void schedule(Schedular *Schedular);
void free_schedular(Schedular *Schedular);