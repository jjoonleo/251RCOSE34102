/***********************************************
 * Operating System Term project               *
 * CPU Scheduling Simulator                    *
 *                                             *
 * Worked by 2020120036 경영학과 오윤진         *
 * Last update: 23/05/30                       *
 *                                             *
 *                                             *
 ***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TIME_UNIT 1000
#define MAX_PROCESS_NUM 20
#define MAX_ALGORITHM_NUM 10
#define MAX_SWITCH_NUM 150
#define TIME_QUANTUM 4
#define AGING_PERIOD 20

#define FCFS 0
#define SJF 1
#define PRIORITY 2
#define RR 3
#define P_SJF 4
#define P_PRIORITY 5

#define TRUE 1
#define FALSE 0

typedef struct process
{

    int pid;
    int priority;
    int arrivalTime;
    int CPUburst;
    int IOburst;
    int CPUremainingTime;
    int IOremainingTime;
    int waitingTime;
    int turnaroundTime;
    int responseTime;

} process;
typedef struct process *processPointer;

int computation_start = 0;
int computation_end = 0;
int computation_idle = 0;

typedef struct evaluation
{

    int alg;
    int preemptive;
    int startTime;
    int endTime;
    int avg_waitingTime;
    int avg_turnaroundTime;
    int avg_responseTime;
    double CPU_util;
    int completed;

} evaluation;
typedef struct evaluation *evalPointer;

evalPointer evals[MAX_ALGORITHM_NUM];

int cur_eval_num = 0;

void init_evals()
{

    cur_eval_num = 0;
    for (int i = 0; i < MAX_ALGORITHM_NUM; i++)
        evals[i] = NULL;
}

void clear_evals()
{

    for (int i = 0; i < MAX_ALGORITHM_NUM; i++)
    {

        free(evals[i]);
        evals[i] = NULL;
    }
    cur_eval_num = 0;
}

typedef struct switching
{

    int new_pid;
    int switching_time;

} switching;
typedef struct switching *switchPointer;

switchPointer switches[MAX_SWITCH_NUM];

int cur_switch_num = 0;

void init_switches()
{

    for (int i = 0; i < MAX_SWITCH_NUM; i++)
        switches[i] = NULL;
}

void clear_switches()
{

    for (int i = 0; i < MAX_SWITCH_NUM; i++)
    {

        free(switches[i]);
        switches[i] = NULL;
    }
}

processPointer jobQueue[MAX_PROCESS_NUM];

int cur_proc_num_JQ = 0;

void init_JQ()
{

    cur_proc_num_JQ = 0;
    int i;
    for (i = 0; i < MAX_PROCESS_NUM; i++)
        jobQueue[i] = NULL;
}

void sort_JQ()
{

    processPointer key;
    int i, j;

    for (i = 1; i < cur_proc_num_JQ; i++)
    {
        key = jobQueue[i];

        for (j = i - 1; (j >= 0) && (jobQueue[j]->pid > key->pid); j--)
        {
            jobQueue[j + 1] = jobQueue[j];
        }
        jobQueue[j + 1] = key;
    }
}

int getProcByPid_JQ(int givenPid)
{

    for (int i = 0; i < cur_proc_num_JQ; i++)
    {
        int temp = jobQueue[i]->pid;
        if (temp == givenPid)
            return i;
    }

    return -1;
}

void insertInto_JQ(processPointer proc)
{

    if (cur_proc_num_JQ < MAX_PROCESS_NUM)
    {

        int temp = getProcByPid_JQ(proc->pid);
        if (temp != -1)
            printf("<ERROR> The process with pid: %d already exists in job queue.\n", proc->pid);
        jobQueue[cur_proc_num_JQ++] = proc;
    }

    else
        printf("<ERROR> Job queue is full.\n");
}

processPointer removeFrom_JQ(processPointer proc)
{

    if (cur_proc_num_JQ > 0)
    {
        int temp = getProcByPid_JQ(proc->pid);

        if (temp == -1)
        {
            printf("<ERROR> Cannot find the process with pid: %d.\n", proc->pid);
            return NULL;
        }

        else
        {
            processPointer removed = jobQueue[temp];
            for (int i = temp; i < cur_proc_num_JQ - 1; i++)
                jobQueue[i] = jobQueue[i + 1];
            jobQueue[cur_proc_num_JQ - 1] = NULL;
            cur_proc_num_JQ--;
            return removed;
        }
    }

    else
    {
        puts("<ERROR> Job queue is empty.");
        return NULL;
    }
}

void clear_JQ()
{

    for (int i = 0; i < MAX_PROCESS_NUM; i++)
    {
        free(jobQueue[i]);
        jobQueue[i] = NULL;
    }

    cur_proc_num_JQ = 0;
}

void print_JQ()
{

    printf("[ Job Queue (total # of process: %d) ]\n", cur_proc_num_JQ);
    printf("pid\t\tpriority\t\tarrival_time\t\tCPU burst\t\tIO burst\n");
    printf("=============================================================================================\n");
    for (int i = 0; i < cur_proc_num_JQ; i++)
    {
        printf("%3d\t\t%8d\t\t%12d\t\t%9d\t\t%8d\n", jobQueue[i]->pid, jobQueue[i]->priority, jobQueue[i]->arrivalTime, jobQueue[i]->CPUburst, jobQueue[i]->IOburst);
    }
    printf("=============================================================================================\n\n\n");
}

processPointer cloneJobQueue[MAX_PROCESS_NUM];
int cur_proc_num_clone_JQ = 0;

void clone_JQ()
{

    for (int i = 0; i < MAX_PROCESS_NUM; i++)
    {
        cloneJobQueue[i] = NULL;
    }

    for (int i = 0; i < cur_proc_num_JQ; i++)
    {
        processPointer newProcess = (processPointer)malloc(sizeof(struct process));

        newProcess->pid = jobQueue[i]->pid;
        newProcess->priority = jobQueue[i]->priority;
        newProcess->arrivalTime = jobQueue[i]->arrivalTime;
        newProcess->CPUburst = jobQueue[i]->CPUburst;
        newProcess->IOburst = jobQueue[i]->IOburst;
        newProcess->CPUremainingTime = jobQueue[i]->CPUremainingTime;
        newProcess->IOremainingTime = jobQueue[i]->IOremainingTime;
        newProcess->waitingTime = jobQueue[i]->waitingTime;
        newProcess->turnaroundTime = jobQueue[i]->turnaroundTime;
        newProcess->responseTime = jobQueue[i]->responseTime;

        cloneJobQueue[i] = newProcess;
    }

    cur_proc_num_clone_JQ = cur_proc_num_JQ;
}

void loadClone_JQ()
{

    clear_JQ();
    for (int i = 0; i < cur_proc_num_clone_JQ; i++)
    {

        processPointer newProcess = (processPointer)malloc(sizeof(struct process));
        newProcess->pid = cloneJobQueue[i]->pid;
        newProcess->priority = cloneJobQueue[i]->priority;
        newProcess->arrivalTime = cloneJobQueue[i]->arrivalTime;
        newProcess->CPUburst = cloneJobQueue[i]->CPUburst;
        newProcess->IOburst = cloneJobQueue[i]->IOburst;
        newProcess->CPUremainingTime = cloneJobQueue[i]->CPUremainingTime;
        newProcess->IOremainingTime = cloneJobQueue[i]->IOremainingTime;
        newProcess->waitingTime = cloneJobQueue[i]->waitingTime;
        newProcess->turnaroundTime = cloneJobQueue[i]->turnaroundTime;
        newProcess->responseTime = cloneJobQueue[i]->responseTime;

        jobQueue[i] = newProcess;
    }

    cur_proc_num_JQ = cur_proc_num_clone_JQ;
}

void clearClone_JQ()
{

    for (int i = 0; i < MAX_PROCESS_NUM; i++)
    {
        free(cloneJobQueue[i]);
        cloneJobQueue[i] = NULL;
    }
}

processPointer runningProcess = NULL;

int timeConsumed = 0;

processPointer readyQueue[MAX_PROCESS_NUM];
int cur_proc_num_RQ = 0;

void init_RQ()
{
    cur_proc_num_RQ = 0;
    for (int i = 0; i < MAX_PROCESS_NUM; i++)
        readyQueue[i] = NULL;
}

int getProcByPid_RQ(int givenPid)
{

    for (int i = 0; i < cur_proc_num_RQ; i++)
    {
        int temp = readyQueue[i]->pid;
        if (temp == givenPid)
            return i;
    }
    return -1;
}

void insertInto_RQ(processPointer proc)
{

    if (cur_proc_num_RQ < MAX_PROCESS_NUM)
    {
        int temp = getProcByPid_RQ(proc->pid);
        if (temp != -1)
        {
            printf("<ERROR> The process with pid: %d already exists in Ready queue\n", proc->pid);
        }
        readyQueue[cur_proc_num_RQ++] = proc;
    }
    else
    {
        printf("<ERROR> Ready queue is full\n");
    }
}

processPointer removeFrom_RQ(processPointer proc)
{

    if (cur_proc_num_RQ > 0)
    {

        int temp = getProcByPid_RQ(proc->pid);
        if (temp == -1)
        {
            printf("<ERROR> Cannot find the process with pid: %d\n", proc->pid);
            return NULL;
        }

        else
        {
            processPointer removed = readyQueue[temp];

            for (int i = temp; i < cur_proc_num_RQ - 1; i++)
                readyQueue[i] = readyQueue[i + 1];
            readyQueue[cur_proc_num_RQ - 1] = NULL;

            cur_proc_num_RQ--;
            return removed;
        }
    }

    else
    {
        printf("<ERROR> Ready queue is empty\n");
        return NULL;
    }
}

void clear_RQ()
{

    for (int i = 0; i < MAX_PROCESS_NUM; i++)
    {
        free(readyQueue[i]);
        readyQueue[i] = NULL;
    }
    cur_proc_num_RQ = 0;
}

processPointer waitingQueue[MAX_PROCESS_NUM];
int cur_proc_num_WQ = 0;

void init_WQ()
{
    cur_proc_num_WQ = 0;
    for (int i = 0; i < MAX_PROCESS_NUM; i++)
        waitingQueue[i] = NULL;
}

int getProcByPid_WQ(int givenPid)
{

    for (int i = 0; i < cur_proc_num_WQ; i++)
    {
        int temp = waitingQueue[i]->pid;
        if (temp == givenPid)
            return i;
    }
    return -1;
}

void insertInto_WQ(processPointer proc)
{

    if (cur_proc_num_WQ < MAX_PROCESS_NUM)
    {
        int temp = getProcByPid_WQ(proc->pid);

        if (temp != -1)
        {
            printf("<ERROR> The process with pid: %d already exists in Waiting queue\n", proc->pid);
        }
        waitingQueue[cur_proc_num_WQ++] = proc;
    }

    else
    {
        printf("<ERROR> Waiting queue is full\n");
    }
}

processPointer removeFrom_WQ(processPointer proc)
{

    if (cur_proc_num_WQ > 0)
    {

        int temp = getProcByPid_WQ(proc->pid);

        if (temp == -1)
        {
            printf("<ERROR> Cannot find the process with pid: %d\n", proc->pid);
            return NULL;
        }

        else
        {
            processPointer removed = waitingQueue[temp];
            for (int i = temp; i < cur_proc_num_WQ - 1; i++)
                waitingQueue[i] = waitingQueue[i + 1];
            waitingQueue[cur_proc_num_WQ - 1] = NULL;
            cur_proc_num_WQ--;
            return removed;
        }
    }

    else
    {
        printf("<ERROR> Waiting queue is empty\n");
        return NULL;
    }
}

void clear_WQ()
{

    for (int i = 0; i < MAX_PROCESS_NUM; i++)
    {
        free(waitingQueue[i]);
        waitingQueue[i] = NULL;
    }
    cur_proc_num_WQ = 0;
}

processPointer terminated[MAX_PROCESS_NUM];
int cur_proc_num_T = 0;

void init_T()
{

    cur_proc_num_T = 0;
    for (int i = 0; i < MAX_PROCESS_NUM; i++)
        terminated[i] = NULL;
}

void clear_T()
{

    for (int i = 0; i < MAX_PROCESS_NUM; i++)
    {
        free(terminated[i]);
        terminated[i] = NULL;
    }
    cur_proc_num_T = 0;
}

void insertInto_T(processPointer proc)
{
    if (cur_proc_num_T < MAX_PROCESS_NUM)
    {
        terminated[cur_proc_num_T++] = proc;
    }
    else
    {
        printf("<ERROR> Cannot terminate the process\n");
    }
}

processPointer createProcess(int pid, int priority, int arrivalTime, int CPUburst, int IOburst)
{

    if (arrivalTime > MAX_TIME_UNIT || arrivalTime < 0)
    {
        printf("<ERROR> arrivalTime should be in [0..MAX_TIME_UNIT]\n");
        return NULL;
    }

    if (CPUburst <= 0 || IOburst < 0)
    {
        printf("<ERROR> CPUburst and should be larger than 0 and IOburst cannot be a negative number.\n");
        return NULL;
    }

    processPointer newProcess = (processPointer)malloc(sizeof(struct process));
    newProcess->pid = pid;
    newProcess->priority = priority;
    newProcess->arrivalTime = arrivalTime;
    newProcess->CPUburst = CPUburst;
    newProcess->IOburst = IOburst;
    newProcess->CPUremainingTime = CPUburst;
    newProcess->IOremainingTime = IOburst;
    newProcess->waitingTime = 0;
    newProcess->turnaroundTime = 0;
    newProcess->responseTime = -1;

    insertInto_JQ(newProcess);

    return newProcess;
}

/* Choose next process according to the specific algorithm - return next processPointer */

processPointer FCFS_alg()
{

    processPointer earliestProc = readyQueue[0];

    if (earliestProc != NULL)
    {

        if (runningProcess != NULL)
            return runningProcess;

        else
            return removeFrom_RQ(earliestProc);
    }

    else
        return runningProcess;
}

processPointer SJF_alg(int preemptive)
{

    processPointer shortestJob = readyQueue[0];

    if (shortestJob != NULL)
    {

        for (int i = 0; i < cur_proc_num_RQ; i++)
        {

            if (readyQueue[i]->CPUremainingTime < shortestJob->CPUremainingTime)
                shortestJob = readyQueue[i];

            else if (readyQueue[i]->CPUremainingTime == shortestJob->CPUremainingTime)
            {

                if (readyQueue[i]->arrivalTime < shortestJob->arrivalTime)
                    shortestJob = readyQueue[i];
            }

            else
            {
                continue;
            }
        }

        if (runningProcess != NULL)
        {

            if (preemptive)
            {

                if (runningProcess->CPUremainingTime > shortestJob->CPUremainingTime)
                {

                    insertInto_RQ(runningProcess);
                    return removeFrom_RQ(shortestJob);
                }

                else if (runningProcess->CPUremainingTime == shortestJob->CPUremainingTime)
                {

                    if (runningProcess->arrivalTime <= shortestJob->arrivalTime)
                        return runningProcess;
                    else
                    {

                        insertInto_RQ(runningProcess);
                        return removeFrom_RQ(shortestJob);
                    }
                }

                else
                    return runningProcess;
            }

            return runningProcess;
        }

        else
            return removeFrom_RQ(shortestJob);
    }

    else
        return runningProcess;
}

processPointer PRIORITY_alg(int amount, int preemptive)
{

    for (int i = 0; i < cur_proc_num_RQ; i++)
    {

        if (readyQueue[i]->priority != 1)

            readyQueue[i]->priority -= (amount - readyQueue[i]->arrivalTime) / AGING_PERIOD;
    }

    processPointer importantJob = readyQueue[0];

    if (importantJob != NULL)
    {

        for (int i = 0; i < cur_proc_num_RQ; i++)
        {

            if (readyQueue[i]->priority < importantJob->priority)
                importantJob = readyQueue[i];

            else if (readyQueue[i]->priority == importantJob->priority)
            {

                if (readyQueue[i]->arrivalTime < importantJob->arrivalTime)
                    importantJob = readyQueue[i];
            }

            else
            {
                continue;
            }
        }

        if (runningProcess != NULL)
        {

            if (preemptive)
            {

                if (runningProcess->priority > importantJob->priority)
                {

                    insertInto_RQ(runningProcess);
                    return removeFrom_RQ(importantJob);
                }

                else if (runningProcess->priority == importantJob->priority)
                {

                    if (runningProcess->arrivalTime <= importantJob->arrivalTime)
                        return runningProcess;
                    else
                    {

                        insertInto_RQ(runningProcess);
                        return removeFrom_RQ(importantJob);
                    }
                }

                else
                    return runningProcess;
            }

            return runningProcess;
        }

        else
        {
            return removeFrom_RQ(importantJob);
        }
    }

    else
    {
        return runningProcess;
    }
}

processPointer RR_alg(int time_quantum)
{

    processPointer earliestProc = readyQueue[0];

    if (earliestProc != NULL)
    {

        if (runningProcess != NULL)
        {

            if (timeConsumed >= TIME_QUANTUM)
            {
                insertInto_RQ(runningProcess);
                return removeFrom_RQ(earliestProc);
            }
            else
                return runningProcess;
        }
        else
            return removeFrom_RQ(earliestProc);
    }

    else
    {
        return runningProcess;
    }
}

processPointer schedule(int amount, int alg, int preemptive, int time_quantum)
{

    processPointer selectedProcess = NULL;

    switch (alg)
    {
    case FCFS:
        selectedProcess = FCFS_alg();
        break;
    case SJF:
        selectedProcess = SJF_alg(preemptive);
        break;
    case RR:
        selectedProcess = RR_alg(time_quantum);
        break;
    case PRIORITY:
        selectedProcess = PRIORITY_alg(amount, preemptive);
        break;
    default:
        return NULL;
    }

    return selectedProcess;
}

void simulate(int amount, int alg, int preemptive, int time_quantum)
{

    processPointer tempProcess = NULL;

    for (int i = 0; i < cur_proc_num_JQ; i++)
    {

        if (jobQueue[i]->arrivalTime == amount)
        {
            tempProcess = removeFrom_JQ(jobQueue[i--]);
            insertInto_RQ(tempProcess);
        }
    }

    processPointer prevProcess = runningProcess;

    runningProcess = schedule(amount, alg, preemptive, time_quantum);

    if (prevProcess != runningProcess)
    {

        switchPointer newSwitch = (switchPointer)malloc(sizeof(struct switching));

        newSwitch->new_pid = runningProcess->pid;
        newSwitch->switching_time = amount;

        switches[cur_switch_num++] = newSwitch;

        timeConsumed = 0;

        if (runningProcess->responseTime == -1)
        {
            runningProcess->responseTime = amount - runningProcess->arrivalTime;
        }
    }

    for (int i = 0; i < cur_proc_num_RQ; i++)
    {

        if (readyQueue[i])
        {
            readyQueue[i]->waitingTime++;
            readyQueue[i]->turnaroundTime++;
        }
    }

    for (int i = 0; i < cur_proc_num_WQ; i++)
    {

        if (waitingQueue[i])
        {
            waitingQueue[i]->turnaroundTime++;
            waitingQueue[i]->IOremainingTime--;

            if (waitingQueue[i]->IOremainingTime <= 0)
            {

                insertInto_RQ(removeFrom_WQ(waitingQueue[i--]));
            }
        }
    }

    if (runningProcess != NULL)
    {

        runningProcess->CPUremainingTime--;
        runningProcess->turnaroundTime++;
        timeConsumed++;

        if (runningProcess->CPUremainingTime <= 0)
        {
            insertInto_T(runningProcess);
            runningProcess = NULL;
        }

        else
        {

            if (runningProcess->IOremainingTime > 0)
            {
                insertInto_WQ(runningProcess);
                runningProcess = NULL;
            }
        }
    }

    else
    {
        if ((amount == 0) || (prevProcess == runningProcess))
        {

            switchPointer newSwitch = (switchPointer)malloc(sizeof(struct switching));

            newSwitch->new_pid = 0;
            newSwitch->switching_time = amount;

            switches[cur_switch_num++] = newSwitch;
        }

        computation_idle++;
    }
}

void analyze(int alg, int preemptive)
{

    int wait_sum = 0;
    int turnaround_sum = 0;
    int response_sum = 0;
    processPointer p = NULL;

    printf("==================================================================\n");
    for (int i = 0; i < cur_proc_num_T; i++)
    {
        p = terminated[i];
        printf("(pid: %d)\n", p->pid);
        printf("waiting time = %d, ", p->waitingTime);
        printf("turnaround time = %d, ", p->turnaroundTime);
        printf("CPU remaining time = %d\n", p->CPUremainingTime);
        printf("IO remaining time = %d\n", p->IOremainingTime);
        printf("response time = %d\n", p->responseTime);
        printf("==================================================================\n");

        wait_sum += p->waitingTime;
        turnaround_sum += p->turnaroundTime;
        response_sum += p->responseTime;
    }

    printf("start time: %d / end time: %d / CPU utilization : %.2lf%% \n", computation_start, computation_end,
           (double)(computation_end - computation_idle) / (computation_end) * 100);

    if (cur_proc_num_T != 0)
    {
        printf("Average waiting time: %d\n", wait_sum / cur_proc_num_T);
        printf("Average turnaround time: %d\n", turnaround_sum / cur_proc_num_T);
        printf("Average response time: %d\n", response_sum / cur_proc_num_T);
    }

    printf("Number of completed process: %d\n", cur_proc_num_T);

    if (cur_proc_num_T != 0)
    {
        evalPointer newEval = (evalPointer)malloc(sizeof(struct evaluation));
        newEval->alg = alg;
        newEval->preemptive = preemptive;

        newEval->startTime = computation_start;
        newEval->endTime = computation_end;
        newEval->avg_waitingTime = wait_sum / cur_proc_num_T;
        newEval->avg_turnaroundTime = turnaround_sum / cur_proc_num_T;
        newEval->avg_responseTime = response_sum / cur_proc_num_T;
        newEval->CPU_util = (double)(computation_end - computation_idle) / (computation_end) * 100;
        newEval->completed = cur_proc_num_T;
        evals[cur_eval_num++] = newEval;
    }
    printf("==================================================================\n\n\n\n");
}

void ganttChart(int alg, int preemptive, int terminate_time)
{

    if ((alg == RR) || (alg == PRIORITY && preemptive == TRUE))
    {
        for (int i = 0; i < terminate_time; i++)
            printf("---");
    }
    else
    {
        for (int i = 0; i < terminate_time; i++)
            printf("--");
    }

    printf(" \n");

    for (int i = 0; i < terminate_time; i++)
    {

        int found = FALSE;
        for (int j = 0; j < cur_switch_num; j++)
        {
            if (switches[j]->switching_time > i)
                break;
            if (switches[j]->switching_time == i)
            {
                found = TRUE;
                if (switches[j]->new_pid == 0)
                    printf("|idle");
                else
                    printf("|P(%d)", switches[j]->new_pid);
            }
        }

        if (!found)
            printf(" ");
    }

    printf("|\n");

    if ((alg == RR) || (alg == PRIORITY && preemptive == TRUE))
    {
        for (int i = 0; i < terminate_time; i++)
            printf("---");
    }
    else
    {
        for (int i = 0; i < terminate_time; i++)
            printf("--");
    }
    printf("\n");

    for (int i = 0; i < terminate_time; i++)
    {

        int found = FALSE;
        for (int j = 0; j < cur_switch_num; j++)
        {
            if (switches[j]->switching_time > i)
                break;
            if (switches[j]->switching_time == i)
            {
                found = TRUE;
                if (switches[j]->new_pid == 0)
                    printf("%d    ", switches[j]->switching_time);
                else
                {
                    if (switches[j]->switching_time > 9)
                        printf("%d     ", switches[j]->switching_time);
                    else
                        printf("%d      ", switches[j]->switching_time);
                }
            }
        }
        if (!found)
            printf(" ");
    }

    printf("%d\n\n\n", terminate_time);
}

void startSimulation(int alg, int preemptive, int time_quantum, int count)
{

    loadClone_JQ();

    cur_switch_num = 0;
    init_switches();

    switch (alg)
    {
    case FCFS:
        printf("[ FCFS Algorithm ]\n\n");
        break;
    case SJF:
        if (preemptive)
            printf("[ Preemptive ");
        else
            printf("[ Non-preemptive ");
        printf("SJF Algorithm ]\n\n");
        break;
    case RR:
        printf("[ Round Robin Algorithm (time quantum: %d) ]\n\n", time_quantum);
        break;
    case PRIORITY:
        if (preemptive)
            printf("[ Preemptive ");
        else
            printf("[ Non-preemptive ");
        printf("Priority Algorithm ]\n\n");
        break;
    default:
        return;
    }

    int initial_proc_num = cur_proc_num_JQ;

    int i;
    if (cur_proc_num_JQ <= 0)
    {
        printf("<ERROR> Simulation failed. Process doesn't exist in the job queue");
        return;
    }

    int minArriv = jobQueue[0]->arrivalTime;

    for (i = 0; i < cur_proc_num_JQ; i++)
    {
        if (minArriv > jobQueue[i]->arrivalTime)
            minArriv = jobQueue[i]->arrivalTime;
    }

    computation_start = minArriv;
    computation_idle = 0;

    for (i = 0; i < count; i++)
    {
        simulate(i, alg, preemptive, TIME_QUANTUM);

        if (cur_proc_num_T == initial_proc_num)
        {
            i++;
            break;
        }
    }
    computation_end = i;

    ganttChart(alg, preemptive, i);
    analyze(alg, preemptive);

    clear_JQ();
    clear_RQ();
    clear_T();
    clear_WQ();
    clear_switches();
    free(runningProcess);
    runningProcess = NULL;
    timeConsumed = 0;
    computation_start = 0;
    computation_end = 0;
    computation_idle = 0;
}

void evaluate()
{

    printf("\n\n[ Total Evaluation ]\n\n");

    for (int i = 0; i < cur_eval_num; i++)
    {

        printf("==================================================================\n");

        int alg = evals[i]->alg;
        int preemptive = evals[i]->preemptive;

        switch (evals[i]->alg)
        {

        case FCFS:
            printf("(1) FCFS Algorithm\n");
            break;
        case SJF:
            if (preemptive)
                printf("(3) Preemptive ");
            else
                printf("(2) Non-preemptive ");
            printf("SJF Algorithm\n");
            break;
        case RR:
            printf("(4) Round Robin Algorithm\n");
            break;
        case PRIORITY:
            if (preemptive)
                printf("(6) Preemptive ");
            else
                printf("(5) Non-preemptive ");
            printf("Priority Algorithm\n");
            break;
        default:

            return;
        }

        printf("start time: %d / end time: %d / CPU utilization : %.2lf%% \n", evals[i]->startTime, evals[i]->endTime, evals[i]->CPU_util);
        printf("Average waiting time: %d\n", evals[i]->avg_waitingTime);
        printf("Average turnaround time: %d\n", evals[i]->avg_turnaroundTime);
        printf("Average response time: %d\n", evals[i]->avg_responseTime);
        printf("Completed: %d\n", evals[i]->completed);
    }
    printf("==================================================================\n\n");
}

void createProcesses(int total_num, int io_num)
{

    srand(time(NULL));

    for (int i = 0; i < total_num; i++)
    {

        createProcess(rand() % 900 + 100, rand() % total_num + 1, rand() % (total_num + 10), rand() % 16 + 5, 0);
    }

    for (int i = 0; i < io_num; i++)
    {

        int randomIdx = rand() % total_num;

        if (jobQueue[randomIdx]->IOburst == 0)
        {

            int randomIOburst = rand() % 10 + 1;
            jobQueue[randomIdx]->IOburst = randomIOburst;
            jobQueue[randomIdx]->IOremainingTime = randomIOburst;
        }

        else
            i--;
    }
    sort_JQ();
    clone_JQ();
}

void main()
{

    init_RQ();
    init_JQ();
    init_T();
    init_WQ();
    init_evals();

    srand(time(NULL));
    int totalProcessNum = 0, totalIOProcessNum = 0;

    while (totalProcessNum < 3)
    {
        totalProcessNum = rand() % MAX_PROCESS_NUM + 1;

        totalIOProcessNum = 3;
    }

    printf("[ Creating Process ]\n");
    printf("Total # of process: %d\n", totalProcessNum);
    printf("Total # of process with IO: %d\n\n\n", totalIOProcessNum);

    createProcesses(totalProcessNum, totalIOProcessNum);
    print_JQ();

    int amount = 150;
    startSimulation(FCFS, FALSE, TIME_QUANTUM, amount);
    startSimulation(SJF, FALSE, TIME_QUANTUM, amount);
    startSimulation(SJF, TRUE, TIME_QUANTUM, amount);
    startSimulation(RR, TRUE, TIME_QUANTUM, amount);
    startSimulation(PRIORITY, FALSE, TIME_QUANTUM, amount);
    startSimulation(PRIORITY, TRUE, TIME_QUANTUM, amount);

    evaluate();

    clear_JQ();
    clear_RQ();
    clear_T();
    clear_WQ();
    clearClone_JQ();
    clear_evals();
}