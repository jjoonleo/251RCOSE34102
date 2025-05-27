#include <stdio.h>
#include "../include/event.h"

Event *new_event(EventType event_type, int time, Process *process)
{
  Event *event = (Event *)malloc(sizeof(Event));
  if (!event)
  {
    perror("Failed to allocate memory for Event");
    exit(EXIT_FAILURE);
  }
  event->event_type = event_type;
  event->time = time;
  event->process = process;
  return event;
}
