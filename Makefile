CC = gcc
CFLAGS = -Iinclude -Wall -g

# Source files
SRC = src/linked_list.c src/process.c scheduler.c

# Output executable
OUT = scheduler

# Default target
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

# Clean up
clean:
	rm -f $(OUT)