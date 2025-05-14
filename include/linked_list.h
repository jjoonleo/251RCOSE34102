// filepath: /linked_list_library/linked_list_library/include/linked_list.h
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

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
  void (*insertInOrder)(struct LinkedList *, void *, int (*)(const void *, const void *));
} LinkedList;

// Function declarations
Node *createNode(void *data);
void appendNode(LinkedList *list, void *data);
void printList(LinkedList *list, void (*printFunc)(void *));
void freeList(LinkedList *list);
LinkedList *createLinkedList();
void deleteFirst(LinkedList *list);
void insertInOrder(LinkedList *list, void *data, int (*compare)(const void *, const void *));

#endif // LINKED_LIST_H