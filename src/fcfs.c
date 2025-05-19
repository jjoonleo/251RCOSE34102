
#include <stdio.h>
#include "../include/process.h"
#include "../include/ready_queue.h"
#include "../include/fcfs.h"
#include "../include/priority_queue.h"

void test(ReadyQueue *queue)
{
  printf("Testing FCFS ready queue\n");
  // print queue->head
  if (queue->head == NULL)
  {
    printf("Queue is empty\n");
  }
  else
  {
    ReadyQueueData *readyQueueData = (ReadyQueueData *)queue->head->data;
    Process *process = (Process *)readyQueueData->process;
    printf("Head process PID: %d\n", process->pid);
  }
}

ReadyQueueData *pop(ReadyQueue *queue)
{
  if (queue->head == NULL)
  {
    return NULL;
  }
  ReadyQueueData *readyQueueData = queue->head->data;
  TreeNode *temp = queue->head;
  
  // For FCFS, we're using a linked list structure even though head is TreeNode*
  // We'll use TreeNode->right as our "next" pointer in the linked list
  queue->head = queue->head->right;
  free(temp);
  return readyQueueData;
}

void push(ReadyQueue *queue, ReadyQueueData *readyQueueData)
{
  Process *process = readyQueueData->process;
  printf("Pushing process %d to the ready queue\n", process->pid);
  
  // Create a new TreeNode (instead of Node)
  TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
  newNode->data = readyQueueData;
  newNode->right = NULL;  // Use right as "next" for linked list
  newNode->left = NULL;   // Not used in FCFS
  newNode->parent = NULL; // Not used in FCFS

  if (queue->head == NULL)
  {
    queue->head = newNode;
  }
  else
  {
    // Traverse to the end of the list using right pointer as "next"
    TreeNode *current = queue->head;
    while (current->right != NULL)
    {
      current = current->right;
    }
    current->right = newNode;
  }
}

bool is_ready_queue_empty(ReadyQueue *queue)
{
  return queue->head == NULL;
}

void print_ready_queue(ReadyQueue *queue)
{
  printf("printing ready queue:\n");
  TreeNode *current = queue->head;
  while (current != NULL)
  {
    ReadyQueueData *readyQueueData = (ReadyQueueData *)current->data;
    Process *process = (Process *)readyQueueData->process;
    process->print(process);
    current = current->right; // Using right pointer as "next"
  }
}

ReadyQueue *new_fcfs_ready_queue()
{
  printf("Creating a new FCFS ready queue\n");
  ReadyQueue *queue = (ReadyQueue *)malloc(sizeof(ReadyQueue));
  if (!queue)
  {
    perror("Failed to allocate memory for ReadyQueue");
    exit(EXIT_FAILURE);
  }
  queue->head = NULL;
  queue->pop = pop;
  queue->push = push;
  queue->is_empty = is_ready_queue_empty;
  queue->print = print_ready_queue;
  queue->test = test;
  return queue;
}