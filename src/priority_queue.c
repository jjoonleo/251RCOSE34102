
#include "priority_queue.h"
#include <stdlib.h>
#include <stdio.h>

static TreeNode *create_node(void *data);
static void free_tree_recursive(TreeNode *node);
static int get_height(TreeNode *node);
static int get_balance_factor(TreeNode *node);
static TreeNode *right_rotate(TreeNode *y);
static TreeNode *left_rotate(TreeNode *x);
static TreeNode *insert_avl(PriorityQueue *queue, TreeNode *root, void *data, TreeNode **new_node);
static TreeNode *find_min_node(TreeNode *node);
static TreeNode *delete_node(PriorityQueue *queue, TreeNode *root, TreeNode *node);

static TreeNode *find_treenode_by_exact_data_ptr(TreeNode *current, void *data_ptr)
{
  if (current == NULL)
  {
    return NULL;
  }
  if (current->data == data_ptr)
  {
    return current;
  }
  TreeNode *found_in_left = find_treenode_by_exact_data_ptr(current->left, data_ptr);
  if (found_in_left != NULL)
  {
    return found_in_left;
  }
  return find_treenode_by_exact_data_ptr(current->right, data_ptr);
}

static TreeNode *create_node(void *data)
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

static void free_tree_recursive(TreeNode *node)
{
  if (node == NULL)
    return;
  free_tree_recursive(node->left);
  free_tree_recursive(node->right);
  free(node);
}

static int get_height(TreeNode *node)
{
  if (node == NULL)
    return 0;

  int left_height = get_height(node->left);
  int right_height = get_height(node->right);

  return (left_height > right_height) ? left_height + 1 : right_height + 1;
}

static int get_balance_factor(TreeNode *node)
{
  if (node == NULL)
    return 0;
  return get_height(node->left) - get_height(node->right);
}

static TreeNode *right_rotate(TreeNode *y)
{
  TreeNode *x = y->left;
  TreeNode *T2 = x->right;

  x->right = y;
  y->left = T2;

  x->parent = y->parent;
  y->parent = x;
  if (T2 != NULL)
    T2->parent = y;

  return x;
}

static TreeNode *left_rotate(TreeNode *x)
{
  TreeNode *y = x->right;
  TreeNode *T2 = y->left;

  y->left = x;
  x->right = T2;

  y->parent = x->parent;
  x->parent = y;
  if (T2 != NULL)
    T2->parent = x;

  return y;
}

static TreeNode *insert_avl(PriorityQueue *queue, TreeNode *root, void *data, TreeNode **new_node)
{

  if (root == NULL)
  {
    *new_node = create_node(data);
    return *new_node;
  }

  int compare_result = queue->cmp(data, root->data);

  if (compare_result <= 0)
  {
    root->left = insert_avl(queue, root->left, data, new_node);
    root->left->parent = root;
  }
  else
  {
    root->right = insert_avl(queue, root->right, data, new_node);
    root->right->parent = root;
  }

  int balance = get_balance_factor(root);

  if (balance > 1 && queue->cmp(data, root->left->data) <= 0)
    return right_rotate(root);

  if (balance < -1 && queue->cmp(data, root->right->data) > 0)
    return left_rotate(root);

  if (balance > 1 && queue->cmp(data, root->left->data) > 0)
  {
    root->left = left_rotate(root->left);
    return right_rotate(root);
  }

  if (balance < -1 && queue->cmp(data, root->right->data) <= 0)
  {
    root->right = right_rotate(root->right);
    return left_rotate(root);
  }

  return root;
}

static TreeNode *find_min_node(TreeNode *node)
{
  TreeNode *current = node;
  while (current->left != NULL)
    current = current->left;
  return current;
}

static TreeNode *delete_node(PriorityQueue *queue, TreeNode *root, TreeNode *node)
{
  if (root == NULL)
    return root;

  if (root == node)
  {

    if (root->left == NULL)
    {
      TreeNode *temp = root->right;
      if (temp != NULL)
        temp->parent = root->parent;
      free(root);
      return temp;
    }
    else if (root->right == NULL)
    {
      TreeNode *temp = root->left;
      if (temp != NULL)
        temp->parent = root->parent;
      free(root);
      return temp;
    }

    TreeNode *temp = find_min_node(root->right);

    root->data = temp->data;

    root->right = delete_node(queue, root->right, temp);
  }
  else
  {

    root->left = delete_node(queue, root->left, node);

    if (root->left != NULL || node->parent == root)
    {
      root->right = delete_node(queue, root->right, node);
    }
  }

  if (root == NULL)
    return root;

  int balance = get_balance_factor(root);

  if (balance > 1 && get_balance_factor(root->left) >= 0)
    return right_rotate(root);

  if (balance > 1 && get_balance_factor(root->left) < 0)
  {
    root->left = left_rotate(root->left);
    return right_rotate(root);
  }

  if (balance < -1 && get_balance_factor(root->right) <= 0)
    return left_rotate(root);

  if (balance < -1 && get_balance_factor(root->right) > 0)
  {
    root->right = right_rotate(root->right);
    return left_rotate(root);
  }

  return root;
}

PriorityQueue *new_priority_queue(int (*cmp)(const void *, const void *))
{
  PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  if (!pq)
    return NULL;

  pq->root = NULL;
  pq->size = 0;
  pq->cmp = cmp;

  pq->push = push_priority_queue;
  pq->top = top_priority_queue;
  pq->pop = pop_priority_queue;
  pq->is_empty = is_empty_priority_queue;
  pq->free = free_priority_queue;
  pq->remove_by_data = remove_priority_queue_by_data;

  return pq;
}

TreeNode *push_priority_queue(PriorityQueue *queue, void *data)
{
  if (!queue)
    return NULL;

  TreeNode *new_node = NULL;
  queue->root = insert_avl(queue, queue->root, data, &new_node);
  if (new_node)
    queue->size++;

  return new_node;
}

void *top_priority_queue(PriorityQueue *queue)
{
  if (!queue || queue->root == NULL)
    return NULL;

  TreeNode *min_node = find_min_node(queue->root);
  if (!min_node)
    return NULL;

  return min_node->data;
}

void *pop_priority_queue(PriorityQueue *queue)
{
  if (!queue || queue->root == NULL || queue->size == 0)
    return NULL;

  TreeNode *min_node = find_min_node(queue->root);
  if (!min_node)
    return NULL;

  void *popped_data = min_node->data;

  queue->root = delete_node(queue, queue->root, min_node);
  queue->size--;

  return popped_data;
}

bool is_empty_priority_queue(PriorityQueue *queue)
{
  if (!queue)
    return true;
  return queue->size == 0;
}

void free_priority_queue(PriorityQueue *queue)
{
  if (!queue)
    return;
  free_tree_recursive(queue->root);
  queue->root = NULL;
  free(queue);
}

void remove_priority_queue_by_data(PriorityQueue *queue, void *data_to_remove)
{
  if (!queue || queue->root == NULL || queue->size == 0 || data_to_remove == NULL)
  {
    return;
  }

  TreeNode *node_to_delete = find_treenode_by_exact_data_ptr(queue->root, data_to_remove);

  if (node_to_delete != NULL)
  {
    queue->root = delete_node(queue, queue->root, node_to_delete);

    queue->size--;
  }
}
