#include "priority_queue.h"
#include "ready_queue.h"
#include "gantt_chart.h"

typedef enum SchedulingAlgorithm
{
  FCFS,
  SJF_PREEMPTIVE,
  SJF_NON_PREEMPTIVE,
  ROUND_ROBIN,
  PRIORITY_PREEMPTIVE,
  PRIORITY_NON_PREEMPTIVE,
  MLFQ
} SchedulingAlgorithm;

typedef struct Schedular
{
  int total_turnaround_time;
  int total_waiting_time;
  int time;
  int time_quantum;
  SchedulingAlgorithm algorithm_type;
  PriorityQueue *event_queue;
  ReadyQueue *ready_queue;
  GanttChart *gantt_chart;
  bool is_preemptive;
  void (*schedule)(struct Schedular *schedular);
  void (*free)(struct Schedular *schedular);
} Schedular;

Schedular *new_schedular(Process *processes[], int process_count, ReadyQueue *ready_queue, int is_preemptive, SchedulingAlgorithm algorithm_type);
Schedular *new_schedular_with_quantum(Process *processes[], int process_count, ReadyQueue *ready_queue, int is_preemptive, int time_quantum, SchedulingAlgorithm algorithm_type);
void schedule(Schedular *Schedular);
void free_schedular(Schedular *Schedular);