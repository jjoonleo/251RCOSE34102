#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/process.h"
#include "include/fcfs.h"
#include "include/sjf.h"
#include "include/round_robin.h"
#include "include/ready_queue.h"
#include "include/priority_queue.h"
#include "include/scheduler.h"
#include "include/priority.h"
#include "include/cfs.h"

#define NUM_PROCESSES 3

int main()
{
  srand(time(NULL));
  Process *processes[NUM_PROCESSES];
  printf("Creating processes:\n");
  for (int i = 0; i < NUM_PROCESSES; i++)
  {
    processes[i] = new_process(i + 1);
    processes[i]->print(processes[i]);
  }

  printf("\n -------------------------------------------------\n\n");

  printf("sjf preemptive\n");
  Schedular *sjf_preemptive_schedular = new_schedular(processes, NUM_PROCESSES, new_sjf_ready_queue(), true);
  sjf_preemptive_schedular->schedule(sjf_preemptive_schedular);
  printf("waiting time: %d\n", sjf_preemptive_schedular->total_waiting_time);
  printf("turnaround time: %d\n", sjf_preemptive_schedular->total_turnaround_time);
  sjf_preemptive_schedular->free(sjf_preemptive_schedular);

  printf("\n ------------------------------------------------\n\n");

  printf("sjf non preemptive\n");
  Schedular *sjf_non_preemptive_schedular = new_schedular(processes, NUM_PROCESSES, new_sjf_ready_queue(), false);
  sjf_non_preemptive_schedular->schedule(sjf_non_preemptive_schedular);
  printf("waiting time: %d\n", sjf_non_preemptive_schedular->total_waiting_time);
  printf("turnaround time: %d\n", sjf_non_preemptive_schedular->total_turnaround_time);
  sjf_non_preemptive_schedular->free(sjf_non_preemptive_schedular);

  printf("\n -------------------------------------------------\n\n");

  printf("fcfs\n");
  Schedular *fcfs_schedular = new_schedular(processes, NUM_PROCESSES, new_fcfs_ready_queue(), false);
  fcfs_schedular->schedule(fcfs_schedular);
  printf("waiting time: %d\n", fcfs_schedular->total_waiting_time);
  printf("turnaround time: %d\n", fcfs_schedular->total_turnaround_time);
  fcfs_schedular->free(fcfs_schedular);

  printf("\n -------------------------------------------------\n\n");

  printf("round robin (quantum = 2)\n");
  Schedular *rr_schedular = new_schedular_with_quantum(processes, NUM_PROCESSES, new_round_robin_ready_queue(), false, 2);
  rr_schedular->schedule(rr_schedular);
  printf("waiting time: %d\n", rr_schedular->total_waiting_time);
  printf("turnaround time: %d\n", rr_schedular->total_turnaround_time);
  rr_schedular->free(rr_schedular);

  printf("\n -------------------------------------------------\n\n");

  printf("round robin (quantum = 4)\n");
  Schedular *rr_schedular2 = new_schedular_with_quantum(processes, NUM_PROCESSES, new_round_robin_ready_queue(), false, 4);
  rr_schedular2->schedule(rr_schedular2);
  printf("waiting time: %d\n", rr_schedular2->total_waiting_time);
  printf("turnaround time: %d\n", rr_schedular2->total_turnaround_time);
  rr_schedular2->free(rr_schedular2);

  printf("\n -------------------------------------------------\n\n");

  printf("priority preemptive\n");
  Schedular *priority_preemptive_schedular = new_schedular(processes, NUM_PROCESSES, new_priority_priority_ready_queue(), true);
  priority_preemptive_schedular->schedule(priority_preemptive_schedular);
  printf("waiting time: %d\n", priority_preemptive_schedular->total_waiting_time);
  printf("turnaround time: %d\n", priority_preemptive_schedular->total_turnaround_time);
  priority_preemptive_schedular->free(priority_preemptive_schedular);

  printf("\n -------------------------------------------------\n\n");

  printf("priority non preemptive\n");
  Schedular *priority_non_preemptive_schedular = new_schedular(processes, NUM_PROCESSES, new_priority_priority_ready_queue(), false);
  priority_non_preemptive_schedular->schedule(priority_non_preemptive_schedular);
  printf("waiting time: %d\n", priority_non_preemptive_schedular->total_waiting_time);
  printf("turnaround time: %d\n", priority_non_preemptive_schedular->total_turnaround_time);
  priority_non_preemptive_schedular->free(priority_non_preemptive_schedular);

  printf("\n -------------------------------------------------\n\n");

  printf("CFS (Completely Fair Scheduler)\n");
  Schedular *cfs_schedular = new_schedular(processes, NUM_PROCESSES, new_cfs_ready_queue(), true);
  cfs_schedular->schedule(cfs_schedular);
  printf("waiting time: %d\n", cfs_schedular->total_waiting_time);
  printf("turnaround time: %d\n", cfs_schedular->total_turnaround_time);
  cfs_schedular->free(cfs_schedular);
}
