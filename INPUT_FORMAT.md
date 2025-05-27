# Process 입력 파일 포맷

```
<number_of_processes>
<arrival_time> <cpu_burst> <priority> <num_io_bursts>
[<io_request_time> <io_burst_time>]
...
```

## Example

```
3
0 8 1 2
3 2
5 1
1 6 2 1
2 3
5 10 0 1
7 2
```

아래와 같은 process 3개 생성:

1. Process 1: arrival time 0, 8 CPU burst time, priority 1, 2 I/O bursts
   - I/O 3에 시작해서 2 동안
   - I/O 5에 시작해서 1 동안
2. Process 2: arrives at time 1, needs 6 CPU units, priority 2, has 1 I/O burst
   - I/O 2에 시작해서 3동안
3. Process 3: arrives at time 5, needs 10 CPU units, priority 0, has 1 I/O burst
   - I/O 7에 시작해서 2동안
