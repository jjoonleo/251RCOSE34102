// filepath: /linked_list_library/linked_list_library/include/linked_list.h
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

// Define a generic node
typedef struct Node
{
  void *data;
  struct Node *next;
} Node;

// Define a LinkedList "class"
typedef struct LinkedList
{
  Node *head;

  void (*append)(struct LinkedList *, void *);
  void (*print)(struct LinkedList *, void (*)(void *));
  void (*free)(struct LinkedList *);
  void (*deleteFirst)(struct LinkedList *);
  void *(*getFirst)(struct LinkedList *);
  void (*insertInOrder)(struct LinkedList *, void *, int (*)(const void *, const void *));
  void (*removeDuplicates)(struct LinkedList *, int (*)(const void *, const void *));
  bool (*is_empty)(struct LinkedList *);
} LinkedList;

// Function declarations
Node *createNode(void *data);
void appendNode(LinkedList *list, void *data);
void printList(LinkedList *list, void (*printFunc)(void *));
void freeList(LinkedList *list);
LinkedList *createLinkedList();
void deleteFirst(LinkedList *list);
void *getFirst(LinkedList *list);
void insertInOrder(LinkedList *list, void *data, int (*compare)(const void *, const void *));
void removeDuplicates(LinkedList *list, int (*compare)(const void *, const void *));
bool is_linked_list_empty(LinkedList *list);

#endif // LINKED_LIST_H