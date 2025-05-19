
#include <stdio.h>
#include "../include/process.h"
#include "../include/ready_queue.h"
#include "../include/fcfs.h"
#include "../include/priority_queue.h"

ReadyQueueData *pop(ReadyQueue *queue)
{
  if (queue->root == NULL)
  {
    return NULL;
  }
  ReadyQueueData *readyQueueData = queue->root->data;
  TreeNode *temp = queue->root;

  // For FCFS, we're using a linked list structure even though head is TreeNode*
  // We'll use TreeNode->right as our "next" pointer in the linked list
  queue->root = queue->root->right;
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

  if (queue->root == NULL)
  {
    queue->root = newNode;
  }
  else
  {
    // Traverse to the end of the list using right pointer as "next"
    TreeNode *current = queue->root;
    while (current->right != NULL)
    {
      current = current->right;
    }
    current->right = newNode;
  }
}

bool is_ready_queue_empty(ReadyQueue *queue)
{
  return queue->root == NULL;
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
  queue->root = NULL;
  queue->pop = pop;
  queue->push = push;
  queue->is_empty = is_ready_queue_empty;
  return queue;
}