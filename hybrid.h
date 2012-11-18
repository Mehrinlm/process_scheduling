#include <iostream>
#include "gantt.h"

#ifndef HYBRIDSCHEDULER
#define HYBRIDSCHEDULER

/*****************************************************
 | Hybrid Scheduler - CLASS DEFINITION               |
 | --------------------------------------------------|
 |                                                   |
 ****************************************************/
class HybridScheduler {
    int time_quantum;
    int processes_scheduled;
    int waiting_time;
    int turnaround_time;
    GanttChart gantt_chart;
    priority_queue<Process*, vector<Process*>, dynamic_priority_cmp>* dynamic_priority_queue;
    priority_queue<Process*, vector<Process*>, io_finish_cmp>* io_queue;
   
  public:
    // CONSTRUCTORS
    HybridScheduler(int);

    // ACCESSORS
    int getTimeQuantum();
    int getProcessesScheduled();
    double getAverageWaitingTime();
    double getAverageTurnaroundTime();
    priority_queue<Process*, vector<Process*>, dynamic_priority_cmp>* getPriorityQueue();
    priority_queue<Process*, vector<Process*>, io_finish_cmp>* getIoQueue();
    GanttChart getGanttChart();

    // MUTATORS
    void addWaitingTime(int);
    void addTurnaroundTime(int);
    void incrementProcessesScheduled();

    // CONDITIONAL INDICATORS
    int allQueuesEmpty();

    // PROCESS ACCESS / MANAGMENT
    Process* getNextProcess();
    void demote(Process*, int);
    void promoteStarvedProcesses(int system_clock);
};


/*****************************************************
 | HybridScheduler - CONSTRUCTOR                     |
 ****************************************************/
HybridScheduler::HybridScheduler(int time_quantum) {

  // set scheduler time quantum
  this->time_quantum = time_quantum;

  // init statistics vars
  processes_scheduled = 0;
  waiting_time = 0;
  turnaround_time = 0;

  // create priority queue
  dynamic_priority_queue = new priority_queue<Process*, vector<Process*>, dynamic_priority_cmp>;

  // create io queue
  io_queue = new priority_queue<Process*, vector<Process*>, io_finish_cmp>;
}


/*****************************************************
 | HybridScheduler - ACCESSORS                       |
 ****************************************************/
int HybridScheduler::getTimeQuantum() {
  return this->time_quantum;
}

int HybridScheduler::getProcessesScheduled() {
  return this->processes_scheduled;
}

double HybridScheduler::getAverageWaitingTime() {
  if (processes_scheduled > 0) {
    return ((double)this->waiting_time)/processes_scheduled;
  } else {
    return 0;
  }
}

double HybridScheduler::getAverageTurnaroundTime() {
  if (processes_scheduled > 0) {
    return ((double)this->turnaround_time)/processes_scheduled;
  } else {
    return 0;
  }
}

priority_queue<Process*, vector<Process*>, dynamic_priority_cmp>* HybridScheduler::getPriorityQueue() {
  return this->dynamic_priority_queue;
}

priority_queue<Process*, vector<Process*>, io_finish_cmp>* HybridScheduler::getIoQueue() {
  return this->io_queue;
}

GanttChart HybridScheduler::getGanttChart() {
  return this->gantt_chart;
}


/*****************************************************
 | HybridScheduler - MUTATORS                        |
 ****************************************************/
void HybridScheduler::incrementProcessesScheduled() {
  this->processes_scheduled++;
}

void HybridScheduler::addWaitingTime(int waiting_time) {
  this->waiting_time += waiting_time;
}

void HybridScheduler::addTurnaroundTime(int turnaround_time) {
  this->turnaround_time += turnaround_time;
}

/*****************************************************
 | HybridScheduler - CONDITIONAL INDICATORS          |
 ****************************************************/
int HybridScheduler::allQueuesEmpty() {
  int all_empty = 1;
  if (!(*dynamic_priority_queue).empty()) {
    all_empty = 0;
  }
  if (!(*io_queue).empty()) {
    all_empty = 0;
  }
  return all_empty;
}

/*****************************************************
 | HybridScheduler - PROCESS ACCESS / MANAGEMENT     |
 ****************************************************/
Process* HybridScheduler::getNextProcess() {
  printf("GET NEXT PROCESS");
  if ((*dynamic_priority_queue).size() > 0) {
    Process* process = (*dynamic_priority_queue).top();
    (*dynamic_priority_queue).pop();
    return process;
  }
  return NULL;

  /*
  for (int i = 0; i < rr_queues.size(); i++) {
    if ((*(rr_queues[i].getQueue())).size() > 0) {
      Process* process = (*(rr_queues[i].getQueue())).top();
      (*(rr_queues[i].getQueue())).pop();
      this->current_queue = i;
      return process;
    }
  }
  if ((*fcfs_queue).size() > 0) {
    Process* process = (*fcfs_queue).top();
    (*fcfs_queue).pop();
    this->current_queue = rr_queues.size();
    return process;
  }
  return NULL;
  */
}

void HybridScheduler::demote(Process *process, int system_clock) {
  printf("DEMOTE PROCESS");
  /* OMG RANDOM STUFFS (MAYBE)
  if (this->current_queue < rr_queues.size() - 1) {
    (*process).setQueueArrival(system_clock);
    (*(rr_queues[this->current_queue + 1].getQueue())).push(process);
  } else {
    (*process).setQueueArrival(system_clock);
    (*fcfs_queue).push(process);
  }
  */
}

void HybridScheduler::promoteStarvedProcesses(int system_clock) {
  printf("PROMOTE STARVED PROCESSES");
  /* OMG RANDOM STUFFS (MAYBE)
  if ((*fcfs_queue).empty() == false){
    while((*fcfs_queue).empty() == false && (*((*fcfs_queue).top())).getQueueArrival() + aging_time == system_clock){
      if (DEBUG) printf("----------->PROMOTING PROCESS %d\tarrived: %d\t systime: %d<-------------\n", (*((*fcfs_queue).top())).getP_ID(), (*((*fcfs_queue).top())).getQueueArrival(), system_clock);
      Process *process = (*fcfs_queue).top();
      (*fcfs_queue).pop();
      (*process).setQueueArrival(system_clock);
      (*(rr_queues[rr_queues.size() - 1]).getQueue()).push(process);
    }
  }
  */
}


/*****************************************************
 | Hybrid Scheduler - (Execution Path)               |
 | --------------------------------------------------|
 |                                                   |
 ****************************************************/
int executeHybrid(std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrival_queue) {

  // set system_clock
  int system_clock = 0;

  cout << "\n----------------------------\nHybrid Scheduler\n----------------------------\n";

  int time_quantum = getUserInt("Enter the scheduler time quantum", 1, INT_MAX);

  // set up var to track current process cpu time (compare to time quantum)
  int cpu_time;

  HybridScheduler hybrid (time_quantum);

  if (DEBUG) cout << "\n----------------------------\nSimulation Start:\n----------------------------\n";

  // print statistics
  cout << "\n----------------------------\nSimulation Statistics:\n----------------------------\n";
  printf("Total Processes Scheduled: %d\n", hybrid.getProcessesScheduled());
  printf("Average Waiting Time: %.2f\n", hybrid.getAverageWaitingTime());
  printf("Average Turnaround Time: %.2f\n", hybrid.getAverageTurnaroundTime());

  // print gantt chart
  hybrid.getGanttChart().print();

  if (DEBUG) cout << "\n----------------------------\nSimulation End\n----------------------------\n";

  /*

  while ((*arrival_queue).empty() == false || mfqs.allQueuesEmpty() == false) {
    cpu_time = 0;

    // declare running process var
    Process* process;
    int process_set = 0;

    //while ((!process_set || (*process).getBurstRemaining() > 0) && cpu_time < mfqs.getCurrentQueueTimeQuantum()) {
    while ((!process_set || (process != 0 && (*process).getBurstRemaining() > 0)) && ((mfqs.currentQueueRR() && cpu_time < mfqs.getCurrentQueueTimeQuantum()) || !mfqs.currentQueueRR())) {

      // retrieve incoming processes
      checkForArrivalsMFQS(arrival_queue, mfqs.getQueue(0), system_clock);

      // check for process promotions from FCFS
      mfqs.promoteStarvedProcesses(system_clock);

      // if no running process, select next process
      if (!process_set) {
        process = mfqs.getNextProcess();
        if (process != 0) {
          mfqs.getGanttChart().start((*process).getP_ID(), system_clock);
        }
        process_set = 1;
      }

      if (DEBUG) {
        printf("Sys_clock: %d\t", system_clock);
        if (process != 0) {
          printf("Process ID: %d\t", (*process).getP_ID());
          printf("Remaining Burst: %d\t", (*process).getBurstRemaining());
          printf("Cpu Time:  %d\t", cpu_time);
          printf("Current Queue: %d\t", mfqs.getCurrentQueue());
        }
        printf("\n");
      }

      // adjust process remaining_burst, cpu_time, and system_clock
      if (process != 0)
        (*process).setBurstRemaining((*process).getBurstRemaining() - 1);
      cpu_time++;
      system_clock++;
    }

    if (process != 0) {
      if ((*process).getBurstRemaining() > 0) {
        mfqs.demote(process, system_clock);
      } else {
        mfqs.incrementProcessesScheduled();
        mfqs.addWaitingTime(system_clock - (*process).getBurst() - (*process).getArrival());
        mfqs.addTurnaroundTime(system_clock - (*process).getArrival());
      }
      mfqs.getGanttChart().end(system_clock);
    }
  }

  */
}

#endif

