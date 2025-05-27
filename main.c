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

void print_menu()
{
  printf("=== Process Scheduling Simulator ===\n");
  printf("Choose process input method:\n");
  printf("1. Manual keyboard input\n");
  printf("2. Load from file\n");
  printf("3. Random generation\n");
  printf("Enter your choice (1-3): ");
}

void cleanup_processes(Process **processes, int num_processes)
{
  if (processes)
  {
    for (int i = 0; i < num_processes; i++)
    {
      if (processes[i] && processes[i]->io_burst_list)
      {

        Node *current = processes[i]->io_burst_list->head;
        while (current)
        {
          Node *next = current->next;
          free(current->data);
          free(current);
          current = next;
        }
        free(processes[i]->io_burst_list);
      }
      free(processes[i]);
    }
    free(processes);
  }
}

int main()
{
  srand(time(NULL));
  Process **processes = NULL;
  int num_processes = 0;
  int choice;

  print_menu();
  scanf("%d", &choice);

  switch (choice)
  {
  case 1:
    printf("\n=== Manual Input ===\n");
    processes = create_processes_keyboard(&num_processes);
    break;

  case 2:
  {
    char filename[256];
    printf("\nEnter filename: ");
    scanf(" %s", filename);
    printf("\n=== Loading from file: %s ===\n", filename);
    processes = create_processes_from_file(filename, &num_processes);
    break;
  }

  case 3:
    printf("\nEnter number of processes for random generation: ");
    scanf("%d", &num_processes);
    printf("\n=== Random Generation ===\n");
    processes = create_processes_random(num_processes);
    break;

  default:
    printf("Invalid choice! Using random generation with 3 processes.\n");
    num_processes = 3;
    processes = create_processes_random(num_processes);
    break;
  }

  if (!processes || num_processes <= 0)
  {
    printf("Error: Could not create processes. Exiting.\n");
    return 1;
  }

  printf("\nCreated processes:\n");
  for (int i = 0; i < num_processes; i++)
  {
    processes[i]->print(processes[i]);
  }

  printf("\n=== Scheduling Algorithm Results ===\n\n");

  printf("sjf preemptive\n");
  Schedular *sjf_preemptive_schedular = new_schedular(processes, num_processes, new_sjf_ready_queue(), true);
  sjf_preemptive_schedular->schedule(sjf_preemptive_schedular);
  printf("average waiting time: %.2f\n", (float)sjf_preemptive_schedular->total_waiting_time / num_processes);
  printf("average turnaround time: %.2f\n", (float)sjf_preemptive_schedular->total_turnaround_time / num_processes);
  sjf_preemptive_schedular->free(sjf_preemptive_schedular);

  printf("\n ------------------------------------------------\n\n");

  printf("sjf non preemptive\n");
  Schedular *sjf_non_preemptive_schedular = new_schedular(processes, num_processes, new_sjf_ready_queue(), false);
  sjf_non_preemptive_schedular->schedule(sjf_non_preemptive_schedular);
  printf("average waiting time: %.2f\n", (float)sjf_non_preemptive_schedular->total_waiting_time / num_processes);
  printf("average turnaround time: %.2f\n", (float)sjf_non_preemptive_schedular->total_turnaround_time / num_processes);
  sjf_non_preemptive_schedular->free(sjf_non_preemptive_schedular);

  printf("\n -------------------------------------------------\n\n");

  printf("fcfs\n");
  Schedular *fcfs_schedular = new_schedular(processes, num_processes, new_fcfs_ready_queue(), false);
  fcfs_schedular->schedule(fcfs_schedular);
  printf("average waiting time: %.2f\n", (float)fcfs_schedular->total_waiting_time / num_processes);
  printf("average turnaround time: %.2f\n", (float)fcfs_schedular->total_turnaround_time / num_processes);
  fcfs_schedular->free(fcfs_schedular);

  printf("\n -------------------------------------------------\n\n");

  printf("round robin (quantum = 2)\n");
  Schedular *rr_schedular = new_schedular_with_quantum(processes, num_processes, new_round_robin_ready_queue(), false, 2);
  rr_schedular->schedule(rr_schedular);
  printf("average waiting time: %.2f\n", (float)rr_schedular->total_waiting_time / num_processes);
  printf("average turnaround time: %.2f\n", (float)rr_schedular->total_turnaround_time / num_processes);
  rr_schedular->free(rr_schedular);

  printf("\n -------------------------------------------------\n\n");

  printf("round robin (quantum = 4)\n");
  Schedular *rr_schedular2 = new_schedular_with_quantum(processes, num_processes, new_round_robin_ready_queue(), false, 4);
  rr_schedular2->schedule(rr_schedular2);
  printf("average waiting time: %.2f\n", (float)rr_schedular2->total_waiting_time / num_processes);
  printf("average turnaround time: %.2f\n", (float)rr_schedular2->total_turnaround_time / num_processes);
  rr_schedular2->free(rr_schedular2);

  printf("\n -------------------------------------------------\n\n");

  printf("priority preemptive\n");
  Schedular *priority_preemptive_schedular = new_schedular(processes, num_processes, new_priority_priority_ready_queue(), true);
  priority_preemptive_schedular->schedule(priority_preemptive_schedular);
  printf("average waiting time: %.2f\n", (float)priority_preemptive_schedular->total_waiting_time / num_processes);
  printf("average turnaround time: %.2f\n", (float)priority_preemptive_schedular->total_turnaround_time / num_processes);
  priority_preemptive_schedular->free(priority_preemptive_schedular);

  printf("\n -------------------------------------------------\n\n");

  printf("priority non preemptive\n");
  Schedular *priority_non_preemptive_schedular = new_schedular(processes, num_processes, new_priority_priority_ready_queue(), false);
  priority_non_preemptive_schedular->schedule(priority_non_preemptive_schedular);
  printf("average waiting time: %.2f\n", (float)priority_non_preemptive_schedular->total_waiting_time / num_processes);
  printf("average turnaround time: %.2f\n", (float)priority_non_preemptive_schedular->total_turnaround_time / num_processes);
  priority_non_preemptive_schedular->free(priority_non_preemptive_schedular);

  printf("\n -------------------------------------------------\n\n");

  printf("CFS (Completely Fair Scheduler)\n");
  Schedular *cfs_schedular = new_schedular(processes, num_processes, new_cfs_ready_queue(), true);
  cfs_schedular->schedule(cfs_schedular);
  printf("average waiting time: %.2f\n", (float)cfs_schedular->total_waiting_time / num_processes);
  printf("average turnaround time: %.2f\n", (float)cfs_schedular->total_turnaround_time / num_processes);
  cfs_schedular->free(cfs_schedular);

  cleanup_processes(processes, num_processes);

  return 0;
}
