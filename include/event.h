#include "process.h"

typedef enum EventType
{
  CPUBurstEnded,
  Arrived,
  IOEnded,
  IORequested,
} EventType;

typedef struct Event
{
  EventType event_type;
  int time;
  Process *process;
} Event;

Event *new_event(EventType event_type, int time, Process *process);