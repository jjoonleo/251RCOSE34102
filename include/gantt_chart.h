#ifndef GANTT_CHART_H
#define GANTT_CHART_H

#include "process.h"

#define BLOCK_SIZE 2

typedef enum
{
  IO,
  CPU,
} GanttSegmentType;

typedef struct GanttSegment
{
  GanttSegmentType type;
  int pid;
  int start_time;
  int end_time;
} GanttSegment;

typedef struct GanttChart
{
  int current_time;
  Process *current_process;
  LinkedList *segments;
  void (*process_ended)(struct GanttChart *gantt_chart, Process *process, int event_time);
  void (*process_started)(struct GanttChart *gantt_chart, Process *process, int event_time);
  void (*io)(struct GanttChart *gantt_chart, Process *process, int start_time, int end_time);
  void (*print_final)(struct GanttChart *gantt_chart);
} GanttChart;

GanttSegment *new_gantt_segment(int pid, GanttSegmentType type, int start_time, int end_time);
GanttChart *new_gantt_chart();

void process_started_gantt_chart(GanttChart *gantt_chart, Process *process, int event_time);
void process_ended_gantt_chart(GanttChart *gantt_chart, Process *process, int event_time);
void io_gantt_chart(GanttChart *gantt_chart, Process *process, int start_time, int end_time);
void print_gantt_chart_final(GanttChart *gantt_chart);

#endif
