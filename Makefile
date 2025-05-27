CC = gcc
CFLAGS = -Iinclude -Wall -g

# Source files
SRC = main.c src/linked_list.c src/process.c src/fcfs.c src/sjf.c src/round_robin.c src/scheduler.c src/priority_queue.c src/event.c src/ready_queue.c src/priority_ready_queue.c src/linked_list_ready_queue.c src/gantt_chart.c src/io_waiting_queue.c src/priority.c src/multi_level_ready_queue.c

# Output executable
OUT = main

# Default target
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

# Clean up
clean:
	rm -f $(OUT)