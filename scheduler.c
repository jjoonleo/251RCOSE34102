#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "include/process.h"
#include "include/fcfs.h"
#include "include/sjf.h"
#include "include/ready_queue.h"
#include "include/priority_queue.h"
#include "include/event.h"

#define INF 0x7FFFFFFF

void print_io_burst(void *data)
{
  IOBurst *io_burst = (IOBurst *)data;
  printf("IO Request Time: %d, IO Burst Time: %d\n", io_burst->io_request_time, io_burst->io_burst_time);
}

// Comparison function for arrival time
int compare_event(const void *a, const void *b)
{
  const Event *p1 = (const Event *)a;
  const Event *p2 = (const Event *)b;
  return p1->time - p2->time;
}

int main()
{
  srand(time(NULL));
  Process *processes[5];
  printf("Creating processes:\n");
  // Use priority queue instead of linked list
  PriorityQueue *event_queue = new_priority_queue(compare_event);
  for (int i = 0; i < 5; i++)
  {
    processes[i] = new_process(i + 1);
    Event *event = (Event *)malloc(sizeof(Event));
    event->event_type = Arrived;
    event->time = processes[i]->arrival_time;
    event->process = processes[i];
    processes[i]->print(processes[i]);

    TreeNode *node = event_queue->push(event_queue, event);
    if (i == 3)
    {
      event->time = 3;                  // Set the last event to a very high time
      event->process->arrival_time = 3; // Set remaining time to INF
      event_queue->node_value_changed(event_queue, node);
    }
  }

  // Print the ordered process queue (by popping all elements)
  printf("\nProcesses ordered by arrival time (using priority queue):\n");
  PriorityQueue *print_queue = new_priority_queue(compare_event);
  while (!event_queue->is_empty(event_queue))
  {
    Event *p = (Event *)event_queue->pop(event_queue);
    print_process(p->process);
    print_queue->push(print_queue, p); // Save for later use
  }
  // Restore event_queue for scheduling
  while (!print_queue->is_empty(print_queue))
  {
    event_queue->push(event_queue, print_queue->pop(print_queue));
  }
  free_priority_queue(print_queue);

  /*
  get from ready queue

  process arrival

  add to ready queue from wating queue
  add to ready queue from processing
  */
  ReadyQueue *ready_queue = new_sjf_ready_queue(); // Use SJF instead of FCFS
  int time = 0;
  bool isCPUIdle = true;
  printf("\n--------------\n\n");
  while (ready_queue->is_empty(ready_queue) == false || !event_queue->is_empty(event_queue))
  {
    if (!event_queue->is_empty(event_queue))
    {
      do
      {
        Event *event = (Event *)event_queue->pop(event_queue);
        Process *process = event->process;
        time = event->time;
        printf("\n\nEvent at time %d: ", time);
        switch (event->event_type)
        {
        case Arrived:
        {
          printf("Process %d arrived.\n", process->pid);
          ReadyQueueData *ready_queue_data = new_ready_queue_data(process, time);
          ready_queue->push(ready_queue, ready_queue_data);
          break;
        }
        case CPUBurstEnded:
          printf("\n\nProcess %d CPU burst ended.\n", process->pid);
          process->print_status(process);
          isCPUIdle = true;
          break;

        case IORequested:
        {
          IOBurst *io_burst = (IOBurst *)process->io_burst_list->getFirst(process->io_burst_list);
          Event *io_event = new_event(IOEnded, time + io_burst->io_burst_time, event->process);

          printf("\n\nProcess %d IO burst requested at time %d\n", event->process->pid, time);
          process->print_status(process);
          event->process->io_burst_list->deleteFirst(event->process->io_burst_list);
          event_queue->push(event_queue, io_event);
          isCPUIdle = true;
          break;
        }

        case IOEnded:
        {
          printf("Process %d IO burst ended at time %d\n", process->pid, time);
          process->print_status(process);
          if (event->process->remaining_time <= 0)
          {
            printf("Process %d finished at time %d\n", process->pid, time);
            free(event); // Free the event after processing
            continue;    // Skip to the next iteration
          }
          ReadyQueueData *ready_queue_data = new_ready_queue_data(process, time);
          ready_queue->push(ready_queue, ready_queue_data);
          break;
        }
        default:
          break;
        }
        free(event); // Free the event after processing
      } while (!event_queue->is_empty(event_queue) && ((Event *)event_queue->top(event_queue))->time == time);
    }
    if (isCPUIdle && !ready_queue->is_empty(ready_queue))
    {
      ReadyQueueData * ready_queue_data= ready_queue->pop(ready_queue);
      Process *p = (Process *)ready_queue_data->process;
      printf("Process %d is selected for CPU at time %d\n", p->pid, time);
      isCPUIdle = false;

      // should be changed in preemptive
      p->waiting_time += time - ready_queue_data->start_time;

      int event_time = time + p->remaining_time;
      EventType event_type = CPUBurstEnded;
      if (!p->io_burst_list->is_empty(p->io_burst_list))
      {
        IOBurst *io_burst = (IOBurst *)p->io_burst_list->getFirst(p->io_burst_list);
        event_time = time + io_burst->io_request_time - (p->cpu_burst - p->remaining_time);
        printf("Process %d IO burst requested at time %d\n", p->pid, event_time);
        p->remaining_time -= event_time - time;
        event_type = IORequested;
      }
      else
      {
        p->remaining_time = 0;
      }

      printf("Process %d is running until time %d\n", p->pid, event_time);

      p->turnaround_time = event_time - p->arrival_time;

      Event *event = new_event(event_type, event_time, p);
      event_queue->push(event_queue, event);
    }
  }
  printf("All processes have been processed.\n");
  free_priority_queue(event_queue);
  free_sjf_ready_queue(ready_queue);
}