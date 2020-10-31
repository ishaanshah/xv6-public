# Assignment 5

## Roll Number - 2019111028

### Implementation of `waitx`
To implement `waitx` system call we need to modify the `proc` structure in `proc.h` and add five fields as follows
- `ctime`: The time at which process was created (set in `allocproc`).
- `rtime`: The time when for which process has run (incremented in `trap.c` for each tick)
- `itime`: The time at which process gets inserted into the queue (set at all places where process is made `RUNNABLE`)
- `wtime`: The total amount of time process has spent waiting in queue (Incremented by `ticks - p->itime` in `sleep` or when process starts `RUNNING`)

Using the above variables, we can easily write a system call which returns the wait time and run time of a process.

### Implementation of `ps`
Implementation of `ps` is done using a `proclist` system call, which iterates over all the processes and prints the necessary attributes.

### Implementation of `FCFS` scheduler
As the `FCFS` scheduler is non-preemptive we have to make sure that we dont `yeild` on each tick of the CPU. We do this by conditionally `yielding` in `trap.c` based on the `SCHEDULER` flag. The actual scheduler code is pretty straightforward, we loop over the whole queue to find a process with lowes `ctime`. Once we find a process we switch to it. If the kernel is compiled using the `DEBUG=TRUE` flag, we can see which process has been selected and we can easily see that the scheduling is done in a `FCFS` manner. If we run the benchmark program in background using `&` and then run another command such as `ps` or `ls` then it doesn't run immediately because it has to wait for all the processes which have been created before. Hence `FCFS` scheduler is not ideal for an interactive system.

### Implementation of `PBS` scheduler
To implement the `PBS` scheduler we need to add another field to the `proc` struct, i.e the `priority`. The `priority` field is set to default value of 60 in `allocproc`. In the actual scheduler code, we loop over the process list to find the proess with least priority and switch to it. To change the priority of a process we have to implement a new system call `set_priority` which takes in a PID and new priority of the process. If the new priority is lower than the current one, then the scheduler is called to make sure that the process with least priority is running. In the benchmark program we set the priority of half of the child processes to 99 and remaining ones to 100. So we compile the kernel with `DEBUG=TRUE` we can see processes of even or odd parity chosen first and the other chosen later which is an indicator of correct functioning of `PBS` scheduler.

### Implementation of `MLFQ` scheduler
To implement `MLFQ` scheduler we need to add more fieds to the `proc` struct as follows
- `cur_q`: The ID of the queue in which process currently is (set to default `0` in `allocproc`)
- `rqtime`: The number of ticks the process has run in current queue (set to `0` whenever queue is changed or process gives up control voluntarily)
- `q[5]`: Array containing the time spent by each process in respective queues.

The scheduler first finds the minimum queue which has a `RUNNABLE` process, and then selects the process with minimum `itime` to simulate the queue. `rqtime` is incremented on each CPU tick for a process and when it exceeds the time slice of the queue, the process gets yielded. At the beginning of each scheduler run, we iterate over all the process to check if process has aged past the max wait time and promote it to a better queue. We also update the `itime` each time a process gets awakened after sleeping. This can be exploited by processes by performing short bursts of CPU intensive tasks and then voluntarily giving up CPU control for a very short time to make sure that it doesnt get demoted to a lower priority queue.

### Benchmarks
The following are the outputs for `time benchmark` command for different schedulers -

#### Round Robin
```
Wait Time: 891
Run Time: 1
```

#### First Come First Serve
```
Wait Time: 0
Run Time: 5
```

#### Priority Based Scheduler
```
Wait Time: 0
Run Time: 4
```
Note: The performance of `PBS` scheduler is highly dependant on the way priorities are assigned in benchmark

#### Multi-Level Feedback Queue
```
Wait Time: 112
Run Time: 3
```
