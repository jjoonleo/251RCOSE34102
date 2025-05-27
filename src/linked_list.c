
#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

Node *new_node(void *data)
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

void append_node(LinkedList *list, void *data)
{
  Node *newNode = new_node(data);
  if (list->head == NULL)
  {
    list->head = newNode;
    list->size = 1;
    return;
  }
  Node *current = list->head;
  while (current->next != NULL)
  {
    current = current->next;
  }
  current->next = newNode;
  list->size++;
}

void append_node_first(LinkedList *list, void *data)
{
  Node *newNode = new_node(data);
  newNode->next = list->head;
  list->head = newNode;
  list->size++;
}

void print_list(LinkedList *list, void (*printFunc)(void *))
{
  Node *current = list->head;
  while (current != NULL)
  {
    printFunc(current->data);
    current = current->next;
  }
  printf("\n");
}

void free_list(LinkedList *list)
{
  Node *current = list->head;
  while (current != NULL)
  {
    Node *temp = current;
    current = current->next;
    free(temp);
  }
  list->head = NULL;
  list->size = 0;
}

void delete_first(LinkedList *list)
{
  if (list->head == NULL)
  {
    return;
  }
  Node *temp = list->head;
  list->head = list->head->next;
  free(temp);
  list->size--;
}

void *get_first(LinkedList *list)
{
  if (list->head == NULL)
  {
    return NULL;
  }
  return list->head->data;
}

void *get_at(LinkedList *list, int index)
{
  Node *current = list->head;
  for (int i = 0; i < index && current != NULL; i++)
  {
    current = current->next;
  }
  return current ? current->data : NULL;
}

void insert_in_order(LinkedList *list, void *data, int (*compare)(const void *, const void *))
{
  Node *newNode = new_node(data);

  if (list->head == NULL || compare(data, list->head->data) < 0)
  {
    newNode->next = list->head;
    list->head = newNode;
    list->size++;
    return;
  }

  Node *current = list->head;
  while (current->next != NULL && compare(data, current->next->data) >= 0)
  {
    current = current->next;
  }

  newNode->next = current->next;
  current->next = newNode;
  list->size++;
}

void remove_duplicates(LinkedList *list, int (*compare)(const void *, const void *))
{
  Node *current = list->head;
  while (current != NULL && current->next != NULL)
  {
    if (compare(current->data, current->next->data) == 0)
    {
      Node *temp = current->next;
      current->next = current->next->next;
      free(temp);
      list->size--;
    }
    else
    {
      current = current->next;
    }
  }
}

bool is_linked_list_empty(LinkedList *list)
{
  return list->head == NULL;
}

void **to_array(LinkedList *list)
{

  void **array = (void **)malloc(list->size * sizeof(void *));
  if (!array)
  {
    perror("Failed to allocate memory for array");
    exit(EXIT_FAILURE);
  }

  Node *current = list->head;
  for (int i = 0; i < list->size; i++)
  {
    array[i] = current->data;
    current = current->next;
  }
  return array;
}

LinkedList *new_linked_list()
{
  LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
  list->head = NULL;
  list->size = 0;
  list->append = append_node;
  list->append_first = append_node_first;
  list->print = print_list;
  list->free = free_list;
  list->delete_first = delete_first;
  list->get_first = get_first;
  list->insert_in_order = insert_in_order;
  list->remove_duplicates = remove_duplicates;
  list->is_empty = is_linked_list_empty;
  list->get_at = get_at;
  list->to_array = to_array;
  return list;
}