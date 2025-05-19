CC = gcc
CFLAGS = -Iinclude -Wall -g

# Source files
SRC = src/linked_list.c src/process.c src/fcfs.c src/sjf.c scheduler.c src/priority_queue.c src/event.c src/ready_queue.c src/priority_ready_queue.c

# Output executable
OUT = scheduler

# Default target
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

# Clean up
clean:
	rm -f $(OUT)