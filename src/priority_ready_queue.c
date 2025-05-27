
#include "ready_queue.h"
#include <stdlib.h>
#include <stdio.h>

static TreeNode *create_node(ReadyQueueData *data);
static void free_tree_recursive(TreeNode *node);

static void left_rotate(ReadyQueue *queue, TreeNode *x);
static void right_rotate(ReadyQueue *queue, TreeNode *y);
static void rb_insert_fixup(ReadyQueue *queue, TreeNode *z);
static void rb_transplant(ReadyQueue *queue, TreeNode *u, TreeNode *v);
static TreeNode *tree_minimum(TreeNode *node);
static void rb_delete_fixup(ReadyQueue *queue, TreeNode *x);
static TreeNode *rb_delete_node(ReadyQueue *queue, TreeNode *z);
static TreeNode *find_node_to_delete(ReadyQueue *queue, TreeNode *root, ReadyQueueData *data);

static TreeNode *create_node(ReadyQueueData *data)
{
  TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
  if (!newNode)
    return NULL;
  newNode->data = data;
  newNode->left = NULL;
  newNode->right = NULL;
  newNode->parent = NULL;
  newNode->color = RED;
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

static void left_rotate(ReadyQueue *queue, TreeNode *x)
{
  TreeNode *y = x->right;
  x->right = y->left;
  if (y->left != NULL)
  {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == NULL)
  {
    queue->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

static void right_rotate(ReadyQueue *queue, TreeNode *y)
{
  TreeNode *x = y->left;
  y->left = x->right;
  if (x->right != NULL)
  {
    x->right->parent = y;
  }
  x->parent = y->parent;
  if (y->parent == NULL)
  {
    queue->root = x;
  }
  else if (y == y->parent->left)
  {
    y->parent->left = x;
  }
  else
  {
    y->parent->right = x;
  }
  x->right = y;
  y->parent = x;
}

static void rb_insert_fixup(ReadyQueue *queue, TreeNode *z)
{
  while (z->parent != NULL && z->parent->color == RED)
  {
    if (z->parent == z->parent->parent->left)
    {
      TreeNode *y = z->parent->parent->right;
      if (y != NULL && y->color == RED)
      {
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->right)
        {
          z = z->parent;
          left_rotate(queue, z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        right_rotate(queue, z->parent->parent);
      }
    }
    else
    {
      TreeNode *y = z->parent->parent->left;
      if (y != NULL && y->color == RED)
      {
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->left)
        {
          z = z->parent;
          right_rotate(queue, z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        left_rotate(queue, z->parent->parent);
      }
    }
    if (z == queue->root)
      break;
  }
  ((TreeNode *)(queue->root))->color = BLACK;
}

void push_priority_ready_queue(ReadyQueue *queue, ReadyQueueData *data)
{
  if (!queue)
    return;

  TreeNode *z = create_node(data);
  if (!z)
    return;

  TreeNode *y = NULL;
  TreeNode *x = queue->root;

  while (x != NULL)
  {
    y = x;
    if (queue->cmp(z->data, x->data) < 0)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }

  z->parent = y;
  if (y == NULL)
  {
    queue->root = z;
  }
  else if (queue->cmp(z->data, y->data) < 0)
  {
    y->left = z;
  }
  else
  {
    y->right = z;
  }

  z->left = NULL;
  z->right = NULL;
  z->color = RED;
  rb_insert_fixup(queue, z);
  queue->size++;
}

static TreeNode *tree_minimum(TreeNode *node)
{
  while (node != NULL && node->left != NULL)
  {
    node = node->left;
  }
  return node;
}

static TreeNode *tree_maximum(TreeNode *node)
{
  while (node != NULL && node->right != NULL)
  {
    node = node->right;
  }
  return node;
}

void *top_priority_ready_queue(ReadyQueue *queue)
{
  if (!queue || queue->root == NULL)
  {
    return NULL;
  }

  TreeNode *min_node = tree_minimum(queue->root);
  return min_node ? min_node->data : NULL;
}

static void rb_transplant(ReadyQueue *queue, TreeNode *u, TreeNode *v)
{
  if (u->parent == NULL)
  {
    queue->root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }
  if (v != NULL)
  {
    v->parent = u->parent;
  }
}

static void rb_delete_fixup(ReadyQueue *queue, TreeNode *x)
{

  while (x != NULL && x != queue->root && x->color == BLACK)
  {
    if (x == x->parent->left)
    {
      TreeNode *w = x->parent->right;
      if (w == NULL)
        break;

      if (w->color == RED)
      {
        w->color = BLACK;
        x->parent->color = RED;
        left_rotate(queue, x->parent);
        w = x->parent->right;
      }

      if ((w->left == NULL || w->left->color == BLACK) &&
          (w->right == NULL || w->right->color == BLACK))
      {
        w->color = RED;
        x = x->parent;
      }
      else
      {
        if (w->right == NULL || w->right->color == BLACK)
        {
          if (w->left != NULL)
            w->left->color = BLACK;
          w->color = RED;
          right_rotate(queue, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        if (w->right != NULL)
          w->right->color = BLACK;
        left_rotate(queue, x->parent);
        x = queue->root;
      }
    }
    else
    {
      TreeNode *w = x->parent->left;
      if (w == NULL)
        break;

      if (w->color == RED)
      {
        w->color = BLACK;
        x->parent->color = RED;
        right_rotate(queue, x->parent);
        w = x->parent->left;
      }
      if ((w->right == NULL || w->right->color == BLACK) &&
          (w->left == NULL || w->left->color == BLACK))
      {
        w->color = RED;
        x = x->parent;
      }
      else
      {
        if (w->left == NULL || w->left->color == BLACK)
        {
          if (w->right != NULL)
            w->right->color = BLACK;
          w->color = RED;
          left_rotate(queue, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        if (w->left != NULL)
          w->left->color = BLACK;
        right_rotate(queue, x->parent);
        x = queue->root;
      }
    }
  }
  if (x != NULL)
  {
    x->color = BLACK;
  }
}

static TreeNode *find_node_to_delete(ReadyQueue *queue, TreeNode *root, ReadyQueueData *data)
{
  TreeNode *current = root;
  while (current != NULL)
  {
    int cmp_val = queue->cmp(data, current->data);
    if (cmp_val == 0)
    {

      return current;
    }
    else if (cmp_val < 0)
    {
      current = current->left;
    }
    else
    {
      current = current->right;
    }
  }
  return NULL;
}

static TreeNode *rb_delete_node(ReadyQueue *queue, TreeNode *z)
{
  if (!z)
    return NULL;

  TreeNode *y = z;
  TreeNode *x = NULL;
  Color y_original_color = y->color;

  if (z->left == NULL)
  {
    x = z->right;
    rb_transplant(queue, z, z->right);
  }
  else if (z->right == NULL)
  {
    x = z->left;
    rb_transplant(queue, z, z->left);
  }
  else
  {
    y = tree_minimum(z->right);
    y_original_color = y->color;
    x = y->right;

    if (y->parent == z)
    {
      if (x != NULL)
        x->parent = y;
    }
    else
    {
      rb_transplant(queue, y, y->right);
      y->right = z->right;
      if (y->right != NULL)
        y->right->parent = y;
    }
    rb_transplant(queue, z, y);
    y->left = z->left;
    if (y->left != NULL)
      y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == BLACK)
  {

    rb_delete_fixup(queue, x);
  }
  queue->size--;
  return z;
}

ReadyQueueData *pop_priority_ready_queue(ReadyQueue *queue)
{
  if (!queue || queue->root == NULL || queue->size == 0)
  {
    return NULL;
  }

  TreeNode *node_to_delete = tree_minimum(queue->root);
  if (!node_to_delete)
  {
    return NULL;
  }

  ReadyQueueData *popped_data = (ReadyQueueData *)node_to_delete->data;
  TreeNode *deleted_node_structure = rb_delete_node(queue, node_to_delete);

  if (deleted_node_structure)
  {

    free(deleted_node_structure);
  }

  return popped_data;
}

bool is_empty_priority_ready_queue(ReadyQueue *queue)
{
  if (!queue)
    return true;
  return queue->size == 0;
}

void free_priority_ready_queue(ReadyQueue *queue)
{
  if (!queue)
    return;

  free_tree_recursive(queue->root);
  queue->root = NULL;
}