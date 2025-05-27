
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

typedef struct Node
{
  void *data;
  struct Node *next;
} Node;

typedef struct LinkedList
{
  Node *head;
  int size;

  void (*append)(struct LinkedList *, void *);
  void (*append_first)(struct LinkedList *, void *);
  void (*print)(struct LinkedList *, void (*)(void *));
  void (*free)(struct LinkedList *);
  void (*delete_first)(struct LinkedList *);
  void *(*get_first)(struct LinkedList *);
  void *(*get_at)(struct LinkedList *, int);
  void (*insert_in_order)(struct LinkedList *, void *, int (*)(const void *, const void *));
  void (*remove_duplicates)(struct LinkedList *, int (*)(const void *, const void *));
  bool (*is_empty)(struct LinkedList *);
  void **(*to_array)(struct LinkedList *);
} LinkedList;

Node *new_node(void *data);
void append_node(LinkedList *list, void *data);
void append_node_first(LinkedList *list, void *data);
void print_list(LinkedList *list, void (*printFunc)(void *));
void free_list(LinkedList *list);
LinkedList *new_linked_list();
void delete_first(LinkedList *list);
void *get_first(LinkedList *list);
void *get_at(LinkedList *list, int index);
void insert_in_order(LinkedList *list, void *data, int (*compare)(const void *, const void *));
void remove_duplicates(LinkedList *list, int (*compare)(const void *, const void *));
bool is_linked_list_empty(LinkedList *list);
void **to_array(LinkedList *list);

#endif