#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/process.h"
#include "../include/fcfs.h"
#include "../include/sjf.h"
#include "../include/round_robin.h"
#include "../include/ready_queue.h"
#include "../include/priority_queue.h"
#include "../include/event.h"
#include "../include/gantt_chart.h"
#include "../include/io_waiting_queue.h"
#include "../include/scheduler.h"
#include "../include/multi_level_ready_queue.h"

#define INF 0x7FFFFFFF

#define MAX(x, y) ((x) > (y) ? (x) : (y))

static void handle_preemption(PriorityQueue *event_queue, ReadyQueue *ready_queue, Event *current_process_event, int time)
{
  event_queue->remove_by_data(event_queue, current_process_event);
  Process *process = current_process_event->process;
  int cpu_time_used = time - current_process_event->time;
  process->remaining_time -= cpu_time_used;

  process->turnaround_time = time - current_process_event->process->arrival_time;

  if (process->remaining_time > 0)
  {
    ReadyQueueData *ready_queue_data = new_ready_queue_data(process, time);
    ready_queue->push(ready_queue, ready_queue_data);
  }
}

int compare_event(const void *a, const void *b)
{
  const Event *p1 = (const Event *)a;
  const Event *p2 = (const Event *)b;
  return p1->time - p2->time;
}

void free_schedular(Schedular *schedular)
{
  if (schedular == NULL)
    return;
  schedular->event_queue->free(schedular->event_queue);
  schedular->ready_queue->free(schedular->ready_queue);
  free(schedular);
}

void schedule(Schedular *this)
{
  bool isCPUIdle = true;
  Event *current_process_event = NULL;
  IOWaitingQueue *io_waiting_queue = new_io_waiting_queue();
  PriorityQueue *event_queue = this->event_queue;
  ReadyQueue *ready_queue = this->ready_queue;

  while (ready_queue->is_empty(ready_queue) == false || !event_queue->is_empty(event_queue))
  {
    if (!event_queue->is_empty(event_queue))
    {
      do
      {
        Event *event = (Event *)event_queue->pop(event_queue);
        Process *process = event->process;
        this->time = event->time;

        switch (event->event_type)
        {
        case Arrived:
        {

          if (this->is_preemptive && current_process_event != NULL)
          {

            this->gantt_chart->process_ended(this->gantt_chart, current_process_event->process, this->time);
            handle_preemption(event_queue, ready_queue, current_process_event, this->time);
            current_process_event = NULL;
            isCPUIdle = true;
          }
          ReadyQueueData *ready_queue_data = new_ready_queue_data(process, this->time);
          ready_queue->push(ready_queue, ready_queue_data);
          break;
        }
        case CPUBurstEnded:

          this->gantt_chart->process_ended(this->gantt_chart, process, this->time);

          if (process->remaining_time <= 0)
          {

            this->total_turnaround_time += process->turnaround_time;
            this->total_waiting_time += process->waiting_time;
          }
          else
          {
            if (this->algorithm_type == MLFQ)
            {
              mlfq_demote_process(process);
            }
            else if (this->algorithm_type == REGRESSIVE_ROUND_ROBIN)
            {
              process->quantum_time_level += 1;
            }
            ReadyQueueData *ready_queue_data = new_ready_queue_data(process, this->time);
            ready_queue->push(ready_queue, ready_queue_data);
          }

          current_process_event = NULL;
          isCPUIdle = true;
          break;

        case IORequested:
        {

          this->gantt_chart->process_ended(this->gantt_chart, process, this->time);

          if (process->io_burst_list->is_empty(process->io_burst_list))
          {

            free(event);
            continue;
          }

          IOBurst *io_burst = (IOBurst *)process->io_burst_list->get_first(process->io_burst_list);
          int io_request_start_time = MAX(io_waiting_queue->available_time, this->time);
          int io_request_end_time = io_waiting_queue->request_io(io_waiting_queue, io_burst, this->time);
          this->gantt_chart->io(this->gantt_chart, process, io_request_start_time, io_request_end_time);
          Event *io_event = new_event(IOEnded, io_request_end_time, event->process);

          process->turnaround_time = this->time + io_burst->io_burst_time - process->arrival_time;
          event->process->io_burst_list->delete_first(event->process->io_burst_list);
          event_queue->push(event_queue, io_event);
          current_process_event = NULL;
          isCPUIdle = true;
          break;
        }

        case IOEnded:
        {

          if (process->remaining_time <= 0)
          {

            this->total_turnaround_time += process->turnaround_time;
            this->total_waiting_time += process->waiting_time;
            free(event);
            continue;
          }

          if (this->is_preemptive && current_process_event != NULL)
          {

            this->gantt_chart->process_ended(this->gantt_chart, current_process_event->process, this->time);
            handle_preemption(event_queue, ready_queue, current_process_event, this->time);
            current_process_event = NULL;
            isCPUIdle = true;
          }

          ReadyQueueData *ready_queue_data = new_ready_queue_data(process, this->time);
          ready_queue->push(ready_queue, ready_queue_data);
          break;
        }
        default:
          break;
        }
        free(event);
      } while (!event_queue->is_empty(event_queue) && ((Event *)event_queue->top(event_queue))->time == this->time);
    }

    if (isCPUIdle && !ready_queue->is_empty(ready_queue))
    {
      ReadyQueueData *ready_queue_data = ready_queue->pop(ready_queue);
      Process *p = (Process *)ready_queue_data->process;

      isCPUIdle = false;

      p->waiting_time += this->time - ready_queue_data->start_time;

      // Calculate response time if this is the first time the process is scheduled
      if (p->response_time == -1) {
        p->response_time = this->time - p->arrival_time;
        this->total_response_time += p->response_time;
      }

      // Time of next event
      int event_time = this->time + p->remaining_time;
      EventType event_type = CPUBurstEnded;

      if (!p->io_burst_list->is_empty(p->io_burst_list))
      {
        IOBurst *io_burst = (IOBurst *)p->io_burst_list->get_first(p->io_burst_list);
        int io_time = this->time + io_burst->io_request_time - (p->cpu_burst - p->remaining_time);
        if (io_time < event_time)
        {
          event_time = io_time;
          event_type = IORequested;
        }
      }

      if (this->algorithm_type == MLFQ)
      {
        int time_quantum = mlfq_get_time_quantum(p->quantum_time_level);
        if (time_quantum > 0)
        {
          int quantum_time = time_quantum + this->time;
          if (quantum_time < event_time)
          {
            event_time = quantum_time;
            event_type = CPUBurstEnded;
          }
        }
      }
      else if (this->time_quantum > 0)
      {
        int quantum_time = this->time + this->time_quantum;
        if (this->algorithm_type == REGRESSIVE_ROUND_ROBIN)
        {
          quantum_time += p->quantum_time_level;
        }
        if (quantum_time < event_time)
        {
          event_time = quantum_time;
          event_type = CPUBurstEnded;
        }
      }

      int cpu_time_used = event_time - this->time;
      p->remaining_time -= cpu_time_used;

      if (event_type == CPUBurstEnded && event_time == this->time + (p->remaining_time + cpu_time_used))
      {
        p->remaining_time = 0;
      }

      p->turnaround_time = event_time - p->arrival_time;

      this->gantt_chart->process_started(this->gantt_chart, p, this->time);

      Event *event = new_event(event_type, event_time, p);
      current_process_event = event;
      event_queue->push(event_queue, event);
    }
  }
  this->gantt_chart->print_final(this->gantt_chart);

  return;
}

Schedular *new_schedular(Process **processes, int process_count, ReadyQueue *ready_queue, int is_preemptive, SchedulingAlgorithm algorithm_type)
{
  Schedular *schedular = (Schedular *)malloc(sizeof(Schedular));
  if (!schedular)
  {
    perror("Failed to allocate memory for Schedular");
    exit(EXIT_FAILURE);
  }
  PriorityQueue *event_queue = new_priority_queue(compare_event);

  for (int i = 0; i < process_count; i++)
  {
    Event *event = (Event *)malloc(sizeof(Event));
    event->event_type = Arrived;
    event->time = processes[i]->arrival_time;
    event->process = processes[i]->copy(processes[i]);
    event_queue->push(event_queue, event);
  }

  schedular->total_turnaround_time = 0;
  schedular->total_waiting_time = 0;
  schedular->total_response_time = 0;
  schedular->time = 0;
  schedular->time_quantum = 0;
  schedular->algorithm_type = algorithm_type;
  schedular->event_queue = event_queue;
  schedular->ready_queue = ready_queue;
  schedular->is_preemptive = is_preemptive;
  schedular->schedule = schedule;
  schedular->free = free_schedular;
  schedular->gantt_chart = new_gantt_chart();
  return schedular;
}

Schedular *new_schedular_with_quantum(Process **processes, int process_count, ReadyQueue *ready_queue, int is_preemptive, int time_quantum, SchedulingAlgorithm algorithm_type)
{
  Schedular *schedular = (Schedular *)malloc(sizeof(Schedular));
  if (!schedular)
  {
    perror("Failed to allocate memory for Schedular");
    exit(EXIT_FAILURE);
  }
  PriorityQueue *event_queue = new_priority_queue(compare_event);

  for (int i = 0; i < process_count; i++)
  {
    Event *event = (Event *)malloc(sizeof(Event));
    event->event_type = Arrived;
    event->time = processes[i]->arrival_time;
    event->process = processes[i]->copy(processes[i]);
    event_queue->push(event_queue, event);
  }

  schedular->total_turnaround_time = 0;
  schedular->total_waiting_time = 0;
  schedular->total_response_time = 0;
  schedular->time = 0;
  schedular->time_quantum = time_quantum;
  schedular->algorithm_type = algorithm_type;
  schedular->event_queue = event_queue;
  schedular->ready_queue = ready_queue;
  schedular->is_preemptive = is_preemptive;
  schedular->schedule = schedule;
  schedular->free = free_schedular;
  schedular->gantt_chart = new_gantt_chart();
  return schedular;
}