#include <stdio.h>
#include "../include/ready_queue.h"

ReadyQueueData *new_ready_queue_data(Process *process, int start_time)
{
  ReadyQueueData *readyQueueData = (ReadyQueueData *)malloc(sizeof(ReadyQueueData));
  if (!readyQueueData)
  {
    perror("Failed to allocate memory for ReadyQueueData");
    exit(EXIT_FAILURE);
  }
  readyQueueData->process = process;
  readyQueueData->start_time = start_time;
  return readyQueueData;
}