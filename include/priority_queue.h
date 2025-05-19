#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdbool.h>

typedef struct TreeNode
{
  void *data;
  struct TreeNode *left;
  struct TreeNode *right;
  struct TreeNode *parent; // Added parent pointer
} TreeNode;

// PriorityQueue struct
typedef struct PriorityQueue
{
  TreeNode *root; // Changed from head (array) to root (tree)
  int size;
  TreeNode *(*push)(struct PriorityQueue *, void *);
  void *(*pop)(struct PriorityQueue *);
  void *(*top)(struct PriorityQueue *);
  bool (*is_empty)(struct PriorityQueue *);
  int (*cmp)(const void *, const void *); // Comparison function pointer
  void (*node_value_changed)(struct PriorityQueue *, TreeNode *);
} PriorityQueue;

PriorityQueue *new_priority_queue(int (*cmp)(const void *, const void *));
void *pop_priority_queue(PriorityQueue *queue);
void *top_priority_queue(PriorityQueue *queue);
TreeNode *push_priority_queue(PriorityQueue *queue, void *data);
bool is_empty_priority_queue(PriorityQueue *queue);
void free_priority_queue(PriorityQueue *queue);
void node_value_changed_priority_queue(PriorityQueue *queue, TreeNode *node);

#endif // PRIORITY_QUEUE_H
