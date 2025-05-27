#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/multi_level_ready_queue.h"
#include "../include/linked_list.h"
#include "../include/ready_queue.h"

ReadyQueue *new_multi_level_ready_queue(LinkedList *ready_queues)
{
  if (ready_queues == NULL)
  {
    return NULL;
  }

  ReadyQueue *queue = (ReadyQueue *)malloc(sizeof(ReadyQueue));
  if (!queue)
  {
    perror("Failed to allocate memory for ReadyQueue (Multi-Level)");
    exit(EXIT_FAILURE);
  }

  queue->root = ready_queues;
  queue->size = 0;

  queue->pop = pop_multi_level_ready_queue;
  queue->push = push_multi_level_ready_queue;
  queue->is_empty = is_multi_level_ready_queue_empty;
  queue->cmp = NULL;
  queue->free = free_multi_level_ready_queue;

  return queue;
}

ReadyQueueData *pop_multi_level_ready_queue(ReadyQueue *queue)
{
  if (queue == NULL || queue->root == NULL)
  {
    return NULL;
  }

  LinkedList *ready_queues = (LinkedList *)queue->root;

  Node *current_node = ready_queues->head;
  while (current_node != NULL)
  {
    ReadyQueue *level_queue = (ReadyQueue *)current_node->data;

    if (level_queue != NULL && !level_queue->is_empty(level_queue))
    {
      ReadyQueueData *data = level_queue->pop(level_queue);
      if (data != NULL)
      {
        queue->size--;
        return data;
      }
    }

    current_node = current_node->next;
  }

  return NULL;
}

void push_multi_level_ready_queue(ReadyQueue *queue, ReadyQueueData *data)
{
  if (queue == NULL || queue->root == NULL || data == NULL)
  {
    return;
  }

  LinkedList *ready_queues = (LinkedList *)queue->root;
  Process *process = data->process;

  int queue_level = process->burst_time_level;

  if (queue_level < 0)
  {
    queue_level = 0;
  }
  else if (queue_level > 2)
  {
    queue_level = 2;
  }

  ReadyQueue *target_queue = (ReadyQueue *)ready_queues->get_at(ready_queues, queue_level);

  if (target_queue != NULL)
  {
    target_queue->push(target_queue, data);
    queue->size++;
  }
  else
  {

    Node *last_node = ready_queues->head;
    while (last_node != NULL && last_node->next != NULL)
    {
      last_node = last_node->next;
    }

    if (last_node != NULL && last_node->data != NULL)
    {
      ReadyQueue *last_queue = (ReadyQueue *)last_node->data;
      last_queue->push(last_queue, data);
      queue->size++;
    }
  }
}

bool is_multi_level_ready_queue_empty(ReadyQueue *queue)
{
  if (queue == NULL || queue->root == NULL)
  {
    return true;
  }

  LinkedList *ready_queues = (LinkedList *)queue->root;

  Node *current_node = ready_queues->head;
  while (current_node != NULL)
  {
    ReadyQueue *level_queue = (ReadyQueue *)current_node->data;

    if (level_queue != NULL && !level_queue->is_empty(level_queue))
    {
      return false;
    }

    current_node = current_node->next;
  }

  return true;
}

void free_multi_level_ready_queue(ReadyQueue *queue)
{
  if (queue == NULL)
  {
    return;
  }

  if (queue->root != NULL)
  {
    LinkedList *ready_queues = (LinkedList *)queue->root;

    Node *current_node = ready_queues->head;
    while (current_node != NULL)
    {
      ReadyQueue *level_queue = (ReadyQueue *)current_node->data;
      if (level_queue != NULL && level_queue->free != NULL)
      {
        level_queue->free(level_queue);
      }
      current_node = current_node->next;
    }

    ready_queues->free(ready_queues);
  }

  free(queue);
}

void mlfq_demote_process(Process *process)
{
  if (process == NULL)
  {
    return;
  }

  if (process->burst_time_level < 2)
  {
    process->burst_time_level++;
  }
}

int mlfq_get_time_quantum(int queue_level)
{
  switch (queue_level)
  {
  case 0:
    return MLFQ_Q0_QUANTUM;
  case 1:
    return MLFQ_Q1_QUANTUM;
  case 2:
    return MLFQ_Q2_QUANTUM;
  default:
    return MLFQ_Q2_QUANTUM;
  }
}
