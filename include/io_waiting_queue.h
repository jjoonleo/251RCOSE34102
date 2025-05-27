#include "process.h"

typedef struct IOWaitingQueue
{
  int available_time;
  int (*request_io)(struct IOWaitingQueue *io_waiting_queue, IOBurst *io_burst, int time);
} IOWaitingQueue;

IOWaitingQueue *new_io_waiting_queue();
int request_io(IOWaitingQueue *io_waiting_queue, IOBurst *io_burst, int time);