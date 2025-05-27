#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "gantt_chart.h"

int compare_gantt_segments(const void *a, const void *b)
{
  const GanttSegment *seg_a = *(const GanttSegment **)a;
  const GanttSegment *seg_b = *(const GanttSegment **)b;

  if (seg_a->pid != seg_b->pid)
  {
    return seg_a->pid - seg_b->pid;
  }
  return seg_a->start_time - seg_b->start_time;
}

GanttChart *new_gantt_chart()
{
  GanttChart *gantt_chart = (GanttChart *)malloc(sizeof(GanttChart));
  if (!gantt_chart)
  {
    perror("Failed to allocate memory for GanttChart");
    exit(EXIT_FAILURE);
  }
  gantt_chart->current_time = 0;
  gantt_chart->current_process = NULL;
  gantt_chart->segments = new_linked_list();
  gantt_chart->process_ended = process_ended_gantt_chart;
  gantt_chart->process_started = process_started_gantt_chart;
  gantt_chart->io = io_gantt_chart;
  gantt_chart->print_final = print_gantt_chart_final;
  return gantt_chart;
}

GanttSegment *new_gantt_segment(int pid, GanttSegmentType ganttSegmentType, int start_time, int end_time)
{
  GanttSegment *segment = (GanttSegment *)malloc(sizeof(GanttSegment));
  if (!segment)
  {
    perror("Failed to allocate memory for GanttSegment");
    exit(EXIT_FAILURE);
  }
  segment->pid = pid;
  segment->type = ganttSegmentType;
  segment->start_time = start_time;
  segment->end_time = end_time;
  return segment;
}

void process_started_gantt_chart(GanttChart *gantt_chart, Process *process, int event_time)
{
  if (gantt_chart == NULL || process == NULL)
  {
    return;
  }
  if (gantt_chart->current_process == NULL)
  {
    GanttSegment *segment = new_gantt_segment(-1, CPU, gantt_chart->current_time, event_time);
    gantt_chart->segments->append(gantt_chart->segments, segment);
  }
  gantt_chart->current_process = process;
  gantt_chart->current_time = event_time;
}

void process_ended_gantt_chart(GanttChart *gantt_chart, Process *process, int event_time)
{
  if (gantt_chart == NULL || process == NULL)
  {
    return;
  }
  if (gantt_chart->current_process == NULL || gantt_chart->current_process->pid != process->pid)
  {
    printf("Error: Process mismatch in Gantt chart. \n");
    return;
  }
  GanttSegment *segment = new_gantt_segment(process->pid, CPU, gantt_chart->current_time, event_time);
  gantt_chart->segments->append(gantt_chart->segments, segment);
  gantt_chart->current_process = NULL;
  gantt_chart->current_time = event_time;
}

void io_gantt_chart(GanttChart *gantt_chart, Process *process, int start_time, int end_time)
{
  if (gantt_chart == NULL || process == NULL)
  {
    return;
  }
  GanttSegment *segment = new_gantt_segment(process->pid, IO, start_time, end_time);
  gantt_chart->segments->append(gantt_chart->segments, segment);
}

void print_gantt_chart_final(GanttChart *gantt_chart)
{
  if (gantt_chart == NULL)
  {
    printf("Gantt Chart data is NULL.\n");
    return;
  }

  int max_chart_time = gantt_chart->current_time;

  int segment_count = gantt_chart->segments->size;
  GanttSegment **segment_array = (GanttSegment **)gantt_chart->segments->to_array(gantt_chart->segments);

  qsort(segment_array, segment_count, sizeof(GanttSegment *), compare_gantt_segments);

  printf("Timeline:\n");
  printf("      ");
  for (int t = 0; t < max_chart_time + 1; ++t)
  {

    printf("%-*d", BLOCK_SIZE + 1, t);
  }
  printf("\n");
  printf("      ");
  for (int t = 0; t < max_chart_time; ++t)
  {

    for (int k = 0; k < BLOCK_SIZE + 1; ++k)
      printf("-");
  }
  printf("\n");

  int previous_pid = -2;
  int previous_time = 0;
  for (int i = 0; i < segment_count; ++i)
  {
    if (segment_array[i]->pid != previous_pid)
    {
      if (segment_array[i]->pid == -1)
      {
        printf("IDLE | ");
      }
      else
      {
        printf("P%-3d | ", segment_array[i]->pid);
      }
      previous_time = 0;
      previous_pid = segment_array[i]->pid;
    }
    for (int t = 0; t < segment_array[i]->start_time - previous_time; ++t)
    {

      for (int k = 0; k < BLOCK_SIZE + 1; ++k)
        printf(" ");
    }
    for (int t = segment_array[i]->start_time; t < segment_array[i]->end_time; ++t)
    {

      for (int k = 0; k < BLOCK_SIZE; ++k)
        printf("%s", segment_array[i]->type == CPU ? "█" : "▒");
      printf(" ");
    }
    previous_time = segment_array[i]->end_time;
    if ((i + 1 < segment_count && segment_array[i + 1]->pid != previous_pid) || i + 1 == segment_count)
    {
      for (int t = 0; t < max_chart_time - segment_array[i]->end_time; ++t)
      {

        for (int k = 0; k < BLOCK_SIZE + 1; ++k)
          printf(" ");
      }
      printf("|\n");
    }
  }

  printf("      ");
  for (int t = 0; t < max_chart_time; ++t)
  {

    for (int k = 0; k < BLOCK_SIZE + 1; ++k)
      printf("-");
  }
  printf("\n");
  printf("Final chart time point: %d\n", max_chart_time);
}
