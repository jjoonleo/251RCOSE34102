#include <stdio.h>
#include "../include/process.h"
#include "../include/ready_queue.h"
#include "../include/linked_list_ready_queue.h"

ReadyQueueData *pop_linked_list_ready_queue(ReadyQueue *queue)
{
  if (queue->root == NULL)
  {
    return NULL;
  }
  ReadyQueueData *readyQueueData = queue->root->data;
  TreeNode *temp = queue->root;

  queue->root = queue->root->right;
  free(temp);
  return readyQueueData;
}

void push_linked_list_ready_queue(ReadyQueue *queue, ReadyQueueData *readyQueueData)
{

  TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
  newNode->data = readyQueueData;
  newNode->right = NULL;
  newNode->left = NULL;
  newNode->parent = NULL;

  if (queue->root == NULL)
  {
    queue->root = newNode;
  }
  else
  {

    TreeNode *current = queue->root;
    while (current->right != NULL)
    {
      current = current->right;
    }
    current->right = newNode;
  }
}

bool is_linked_list_ready_queue_empty(ReadyQueue *queue)
{
  return queue->root == NULL;
}

void free_linked_list_ready_queue(ReadyQueue *queue)
{
  if (queue == NULL)
    return;

  TreeNode *current = queue->root;
  while (current != NULL)
  {
    TreeNode *temp = current;
    current = current->right;
    free(temp->data);
    free(temp);
  }
  free(queue);
}