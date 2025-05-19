// filepath: /Users/ejunpark/Development/251RCOSE34102/src/priority_ready_queue.c
#include "ready_queue.h"
#include <stdlib.h> // For malloc, free
#include <stdio.h>  // For NULL

// Forward declarations for helper functions
static TreeNode *create_node(ReadyQueueData *data);
static void free_tree_recursive(TreeNode *node);
static void swap_node_data(TreeNode *node1, TreeNode *node2);
static void heapify_up_tree(ReadyQueue *queue, TreeNode *node);
static void heapify_down_tree(ReadyQueue *queue, TreeNode *node);
static TreeNode *get_last_node(ReadyQueue *queue);
static TreeNode *get_node_by_path(TreeNode *root, int path, int depth);

// Helper function to create a new TreeNode
static TreeNode *create_node(ReadyQueueData *data)
{
  TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
  if (!newNode)
    return NULL;
  newNode->data = data;
  newNode->left = NULL;
  newNode->right = NULL;
  newNode->parent = NULL;
  return newNode;
}

// Helper function to recursively free the tree
static void free_tree_recursive(TreeNode *node)
{
  if (node == NULL)
    return;
  free_tree_recursive(node->left);
  free_tree_recursive(node->right);
  free(node);
}

// Helper function to swap data between two nodes
static void swap_node_data(TreeNode *node1, TreeNode *node2)
{
  void *temp_data = node1->data;
  node1->data = node2->data;
  node2->data = temp_data;
}

// Helper function: Maintain min-heap property by moving a node up
static void heapify_up_tree(ReadyQueue *queue, TreeNode *node)
{
  if (node == NULL || node->parent == NULL)
    return;
  // While node is not root and node's data is smaller than its parent's data
  while (node->parent != NULL && queue->cmp(node->data, node->parent->data) < 0)
  {
    swap_node_data(node, node->parent);
    node = node->parent; // Move up to the parent
  }
}

// Helper function: Maintain min-heap property by moving a node down
static void heapify_down_tree(ReadyQueue *queue, TreeNode *node)
{
  if (node == NULL)
    return;
  TreeNode *smallest = node;
  TreeNode *left = node->left;
  TreeNode *right = node->right;

  // Find the smallest among node, left child, and right child
  if (left != NULL && queue->cmp(left->data, smallest->data) < 0)
  {
    smallest = left;
  }
  if (right != NULL && queue->cmp(right->data, smallest->data) < 0)
  {
    smallest = right;
  }

  // If smallest is not the current node, swap and continue heapifying down
  if (smallest != node)
  {
    swap_node_data(node, smallest);
    heapify_down_tree(queue, smallest);
  }
}

// Helper function to get a node based on a path from the root
// path is an integer where bits represent left (0) or right (1) turns
// depth is the number of levels to traverse (root is depth 0)
static TreeNode *get_node_by_path(TreeNode *root, int path_val, int depth)
{
  TreeNode *current = root;
  if (depth == 0 && path_val == 0)
    return root; // Special case for root itself if size is 1
  // Iterate from the (depth-1)-th bit down to the 0-th bit
  for (int i = depth - 1; i >= 0; i--)
  {
    if (current == NULL)
      return NULL; // Path is invalid
    if ((path_val >> i) & 1)
    { // If the bit is 1, go right
      current = current->right;
    }
    else
    { // If the bit is 0, go left
      current = current->left;
    }
  }
  return current;
}

// Helper function to find the last node in the heap (for removal or insertion point)
// This uses the size to determine the path to the last node in a complete binary tree.
static TreeNode *get_last_node(ReadyQueue *queue)
{
  if (queue->root == NULL || queue->size == 0)
    return NULL;
  if (queue->size == 1)
    return queue->root;

  // Calculate depth of the last level. The root is at depth 0.
  // A complete tree of size N has depth floor(log2(N)).
  // However, we need the path to the N-th node (1-indexed for path calculation).
  int n = queue->size;
  int depth = 0;
  int temp_n = n;
  while (temp_n > 1)
  {
    temp_n >>= 1;
    depth++;
  }
  // The path to the N-th node (1-indexed) is (N - (1 << depth)).
  // (1 << depth) is 2^depth, which is the first node index at that depth.
  // Example: size 5. depth = 2. path = 5 - (1<<2) = 5 - 4 = 1 (binary 01).
  // Path from root: for depth 2, means 2 steps. Bit 1 (0) -> left, Bit 0 (1) -> right.
  // So, root -> left -> right.
  // The path value needs to be interpreted for 'depth' steps.
  int path_val = n - (1 << depth);
  return get_node_by_path(queue->root, path_val, depth);
}

// Implementation of push_priority_ready_queue
void push_priority_ready_queue(ReadyQueue *queue, ReadyQueueData *data)
{
  if (!queue)
    return;

  TreeNode *newNode = create_node(data);
  if (!newNode)
    return; // Failed to create node

  queue->size++;
  if (queue->root == NULL)
  {
    queue->root = newNode;
    return; // First node, no parent
  }

  // Find the parent of the new node (which will be the last node's parent or similar logic)
  // The new node is inserted at the first available spot in the last level (or a new level)
  // This is equivalent to finding the parent of the (size)-th node.
  int n = queue->size;
  int depth = 0;
  int temp_n = n;
  while (temp_n > 1)
  {
    temp_n >>= 1;
    depth++;
  }
  // Path to the (n/2)-th node, which is the parent.
  // Example: size = 2. n=2. depth=1. parent_path_val = (2/2) - (1 << (depth_of_parent = 0)) = 1 - 1 = 0.
  // Parent is root. Node goes to root->left.
  // Example: size = 3. n=3. depth=1. parent_path_val = (3/2) - (1 << 0) = 1 - 1 = 0.
  // Parent is root. Node goes to root->right.
  // Example: size = 4. n=4. depth=2. parent_path_val = (4/2) - (1 << (depth_of_parent=1)) = 2 - 2 = 0.
  // Parent is root->left. Node goes to (root->left)->left.

  int parent_n = n / 2;
  if (parent_n == 0)
  { // Should not happen if size was >=1 before increment
    // This case is covered by queue->root == NULL check if size becomes 1.
    // If size becomes > 1, parent_n will be >= 1.
  }

  int parent_depth = 0;
  if (parent_n > 0)
  {
    temp_n = parent_n;
    while (temp_n > 1)
    {
      temp_n >>= 1;
      parent_depth++;
    }
  }
  int parent_path_val = (parent_n > 0) ? parent_n - (1 << parent_depth) : 0;
  TreeNode *parent_node = get_node_by_path(queue->root, parent_path_val, parent_depth);

  newNode->parent = parent_node;
  if (parent_node != NULL)
  { // Should always be true if size > 1
    if (n % 2 == 0)
    { // New node is a left child
      parent_node->left = newNode;
    }
    else
    { // New node is a right child
      parent_node->right = newNode;
    }
  }

  heapify_up_tree(queue, newNode);

  return;
}

void *top_priority_ready_queue(ReadyQueue *queue)
{
  if (!queue || queue->root == NULL)
    return NULL;
  return queue->root->data; // Return the data at the root
}

// Implementation of pop_priority_ready_queue
ReadyQueueData *pop_priority_ready_queue(ReadyQueue *queue)
{
  if (!queue || queue->root == NULL || queue->size == 0)
    return NULL;

  ReadyQueueData *popped_data = (ReadyQueueData *)queue->root->data;

  if (queue->size == 1)
  {
    free(queue->root);
    queue->root = NULL;
    queue->size = 0;
    return popped_data;
  }

  // Get the last node in the heap
  TreeNode *last_node = get_last_node(queue);
  if (!last_node)
  {
    // This should not happen if size > 1
    // Potentially indicates an issue with get_last_node or queue state
    queue->size = 0; // Reset to a safe state
    queue->root = NULL;
    return popped_data; // Or handle as an error
  }

  // Move data from the last node to the root
  queue->root->data = last_node->data;

  // Remove the last node
  if (last_node->parent)
  {
    if (last_node->parent->left == last_node)
    {
      last_node->parent->left = NULL;
    }
    else
    {
      last_node->parent->right = NULL;
    }
  }
  free(last_node);
  queue->size--;

  if (queue->root)
  { // Only heapify if root still exists (size > 0)
    heapify_down_tree(queue, queue->root);
  }

  return popped_data;
}

// Implementation of is_empty_priority_ready_queue
bool is_empty_priority_ready_queue(ReadyQueue *queue)
{
  if (!queue)
    return true;
  return queue->size == 0;
}

// Implementation of free_priority_ready_queue
void free_priority_ready_queue(ReadyQueue *queue)
{
  if (!queue)
    return;
  free_tree_recursive(queue->root);
  queue->root = NULL; // Good practice
  free(queue);
}