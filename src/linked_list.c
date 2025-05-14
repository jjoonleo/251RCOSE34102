// filepath: /linked_list_library/linked_list_library/src/linked_list.c
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

// Create a new node
Node *createNode(void *data)
{
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (!newNode)
  {
    perror("Failed to allocate memory");
    exit(EXIT_FAILURE);
  }
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

// Append a node to the linked list
void appendNode(LinkedList *list, void *data)
{
  Node *newNode = createNode(data);
  if (list->head == NULL)
  {
    list->head = newNode;
    return;
  }
  Node *current = list->head;
  while (current->next != NULL)
  {
    current = current->next;
  }
  current->next = newNode;
}

// Print the linked list (requires a custom print function)
void printList(LinkedList *list, void (*printFunc)(void *))
{
  Node *current = list->head;
  while (current != NULL)
  {
    printFunc(current->data);
    current = current->next;
  }
  printf("\n");
}

// Free the linked list
void freeList(LinkedList *list)
{
  Node *current = list->head;
  while (current != NULL)
  {
    Node *temp = current;
    current = current->next;
    free(temp);
  }
  list->head = NULL;
}

// Delete the first node from the linked list
void deleteFirst(LinkedList *list)
{
  if (list->head == NULL)
  {
    return; // List is already empty
  }
  Node *temp = list->head;
  list->head = list->head->next;
  free(temp);
}

// Insert a node into the linked list in ascending order
void insertInOrder(LinkedList *list, void *data, int (*compare)(const void *, const void *))
{
  Node *newNode = createNode(data);

  // If the list is empty or the new node should be the first node
  if (list->head == NULL || compare(data, list->head->data) < 0)
  {
    newNode->next = list->head;
    list->head = newNode;
    return;
  }

  // Traverse the list to find the correct position
  Node *current = list->head;
  while (current->next != NULL && compare(data, current->next->data) >= 0)
  {
    current = current->next;
  }

  // Insert the new node
  newNode->next = current->next;
  current->next = newNode;
}

// Initialize the LinkedList "class"
LinkedList *createLinkedList()
{
  LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
  list->head = NULL;
  list->append = appendNode;
  list->print = printList;
  list->free = freeList;
  list->deleteFirst = deleteFirst;
  list->insertInOrder = insertInOrder;
  return list;
}