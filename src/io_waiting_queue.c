#include <stdio.h>

#include "../include/io_waiting_queue.h"
#include "../include/process.h"

#define MAX(x, y) ((x) > (y) ? (x) : (y))

IOWaitingQueue *new_io_waiting_queue()
{
  IOWaitingQueue *io_waiting_queue = malloc(sizeof(IOWaitingQueue));
  if (io_waiting_queue == NULL)
  {
    fprintf(stderr, "Error allocating memory for IOWaitingQueue\n");
    exit(EXIT_FAILURE);
  }
  io_waiting_queue->available_time = 0;
  io_waiting_queue->request_io = request_io;
  return io_waiting_queue;
}

int request_io(IOWaitingQueue *io_waiting_queue, IOBurst *io_burst, int time)
{
  if (io_waiting_queue == NULL || io_burst == NULL)
  {
    fprintf(stderr, "Error: IOWaitingQueue or IOBurst is NULL\n");
    return -1;
  }
  io_waiting_queue->available_time = MAX(time, io_waiting_queue->available_time) + io_burst->io_burst_time;
  return io_waiting_queue->available_time;
}