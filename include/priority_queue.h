#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdbool.h>

typedef enum
{
  RED,
  BLACK
} Color;

typedef struct TreeNode
{
  void *data;
  struct TreeNode *left;
  struct TreeNode *right;
  struct TreeNode *parent;
  Color color;
} TreeNode;

typedef struct PriorityQueue
{
  TreeNode *root;
  int size;
  TreeNode *(*push)(struct PriorityQueue *, void *);
  void *(*pop)(struct PriorityQueue *);
  void *(*top)(struct PriorityQueue *);
  bool (*is_empty)(struct PriorityQueue *);
  int (*cmp)(const void *, const void *);
  void (*free)(struct PriorityQueue *);
  void (*remove_by_data)(struct PriorityQueue *, void *);
} PriorityQueue;

PriorityQueue *new_priority_queue(int (*cmp)(const void *, const void *));
void *pop_priority_queue(PriorityQueue *queue);
void *top_priority_queue(PriorityQueue *queue);
TreeNode *push_priority_queue(PriorityQueue *queue, void *data);
bool is_empty_priority_queue(PriorityQueue *queue);
void free_priority_queue(PriorityQueue *queue);
void remove_priority_queue_by_data(PriorityQueue *queue, void *node);

#endif
