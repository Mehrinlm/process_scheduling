#include <iostream>
#include "rr_queue.h"
#include "fcfs_queue.h"
#include "gantt.h"

#ifndef MFQS
#define MFQS

class Mfqs {
    vector<RR_Queue> rr_queues;
    priority_queue<Process*, vector<Process*>, arrive_cmp>* fcfs_queue;
    GanttChart gantt_chart;
    int aging_time;
    int current_queue;

    int processes_scheduled;
    int waiting_time;
    int turnaround_time;
   
  public:
    Mfqs(int, int, int, priority_queue<Process*, vector<Process*>, arrive_cmp>*);
    vector<RR_Queue> getRR_queues();
    priority_queue<Process*, vector<Process*>, arrive_cmp>* getFCFS_queue();
    priority_queue<Process*, vector<Process*>, arrive_cmp>* getQueue(int);
    int aboveQueuesEmpty();
    int allQueuesEmpty();
    int getCurrentQueueTimeQuantum();
    int getCurrentQueue();
    Process* getNextProcess();
    void demote(Process*);
    void addWaitingTime(int);
    void addTurnaroundTime(int);
    void incrementProcessesScheduled();
    double getAverageTurnaroundTime();
    double getAverageWaitingTime();
    int getProcessesScheduled();
    int currentQueueRR();
    GanttChart getGanttChart();
};

Mfqs::Mfqs(int num_queues, int time_quantum, int aging_time, priority_queue<Process*, vector<Process*>, arrive_cmp>* arrival_queue) {

  // set current queue to topmost queue
  current_queue = 0;
  processes_scheduled = 0;
  waiting_time = 0;
  turnaround_time = 0;
  this->aging_time = aging_time;

  // create fcfs_queue
  fcfs_queue = new priority_queue<Process*, vector<Process*>, arrive_cmp>;

  for (int i = 0; i < num_queues - 1; i++) {
    rr_queues.push_back(RR_Queue(time_quantum));
    time_quantum *= 2;
  }
  
  if (DEBUG) {
    cout << "\n\nRR_Queues Created:\n";
    for (int i = 1; i < rr_queues.size() + 1; i++) {
      cout << "Queue #" << i <<  "\tQuantum: " << rr_queues[i - 1].getTime_quantum() << "\n";
    }
  }
}

GanttChart Mfqs::getGanttChart() {
  return this->gantt_chart;
}

vector<RR_Queue> Mfqs::getRR_queues() {
  return this->rr_queues;
}

priority_queue<Process*, vector<Process*>, arrive_cmp>* Mfqs::getFCFS_queue() {
  return this->fcfs_queue;
}

priority_queue<Process*, vector<Process*>, arrive_cmp>* Mfqs::getQueue(int i) {
  if (i < rr_queues.size()) {
    return rr_queues[i].getQueue();
  }
  return fcfs_queue;
}

int Mfqs::aboveQueuesEmpty() {
  int above_empty = 1;
  int i = 0;
  for (int i = 0; i < current_queue; i++) {
    if (!((*(getQueue(i))).empty())) {
      above_empty = 0;
    }
  }
  return above_empty;
}

int Mfqs::getCurrentQueueTimeQuantum() {
  return this->rr_queues[this->current_queue].getTime_quantum();
}

int Mfqs::getCurrentQueue() {
  return this->current_queue;
}

void Mfqs::addWaitingTime(int waiting_time) {
  this->waiting_time += waiting_time;
}

void Mfqs::addTurnaroundTime(int turnaround_time) {
  this->turnaround_time += turnaround_time;
}

void Mfqs::incrementProcessesScheduled() {
  this->processes_scheduled++;
}

double Mfqs::getAverageTurnaroundTime() {
  if (processes_scheduled > 0) {
    return ((double)this->turnaround_time)/processes_scheduled;
  } else {
    return 0;
  }
}

double Mfqs::getAverageWaitingTime() {
  if (processes_scheduled > 0) {
    return ((double)this->waiting_time)/processes_scheduled;
  } else {
    return 0;
  }
}

int Mfqs::getProcessesScheduled() {
  return this->processes_scheduled;
}

Process* Mfqs::getNextProcess() {
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
}

int Mfqs::allQueuesEmpty() {
  int all_empty = 1;
  int i = 0;
  for (int i = 0; i < this->rr_queues.size(); i++) {
    if (!((*(getQueue(i))).empty())) {
      all_empty = 0;
    }
  }
  if (!(*fcfs_queue).empty()) {
    all_empty = 0;
  }
  return all_empty;
}

void Mfqs::demote(Process *process) {
  if (this->current_queue < rr_queues.size() - 1) {
    (*(rr_queues[this->current_queue + 1].getQueue())).push(process);
  } else {
    (*fcfs_queue).push(process);
  }
}

int Mfqs::currentQueueRR() {
  if (this->current_queue < rr_queues.size()) {
    return 1;
  }
  return 0;
}

/*****************************************************
 | Multi-level Feedback Queue - (Execution Path)     |
 | --------------------------------------------------|
 |                                                   |
 ****************************************************/
int executeMFQS(std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrival_queue) {

  // set system_clock
  int system_clock = 0;

  // set cpu-holding queue
  int current_queue = 0;

  // printArrival(*arrival_queue);

  cout << "\n----------------------------\nMulti-level Feedback Queue\n----------------------------\n";

  int num_queues = getUserInt("Enter the number of queues (1-5)", 1, 5);
  int time_quantum = getUserInt("Enter the top queue's time quantum", 1, INT_MAX);
  int aging_time = getUserInt("Enter the aging time", 1, INT_MAX);

  Mfqs mfqs (num_queues, time_quantum, aging_time, arrival_queue);
  
  // set up var to track current process cpu time (compare to time quantum)
  int cpu_time;

  if (DEBUG) cout << "\n----------------------------\nSimulation Start:\n----------------------------\n";

  while ((*arrival_queue).empty() == false || mfqs.allQueuesEmpty() == false) {
    cpu_time = 0;

    // declare running process var
    Process* process;
    int process_set = 0;

    //while ((!process_set || (*process).getBurstRemaining() > 0) && cpu_time < mfqs.getCurrentQueueTimeQuantum()) {
    while ((!process_set || (process != 0 && (*process).getBurstRemaining() > 0)) && ((mfqs.currentQueueRR() && cpu_time < mfqs.getCurrentQueueTimeQuantum()) || !mfqs.currentQueueRR())) {

      // retrieve incoming processes
      checkForArrivalsMFQS(arrival_queue, mfqs.getQueue(0), system_clock);

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
        mfqs.demote(process);
      } else {
        mfqs.incrementProcessesScheduled();
        mfqs.addWaitingTime(system_clock - (*process).getBurst() - (*process).getArrival());
        mfqs.addTurnaroundTime(system_clock - (*process).getArrival());
      }
      mfqs.getGanttChart().end(system_clock);
    }
  }

  // print statistics
  cout << "\n----------------------------\nSimulation Statistics:\n----------------------------\n";
  printf("Total Processes Scheduled: %d\n", mfqs.getProcessesScheduled());
  printf("Average Waiting Time: %.2f\n", mfqs.getAverageWaitingTime());
  printf("Average Turnaround Time: %.2f\n", mfqs.getAverageTurnaroundTime());

  // print gantt chart
  mfqs.getGanttChart().print();

  if (DEBUG) cout << "\n----------------------------\nSimulation End\n----------------------------\n";
}

#endif

