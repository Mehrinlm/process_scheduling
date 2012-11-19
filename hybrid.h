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
    unsigned long int waiting_time;
    unsigned long int turnaround_time;
    GanttChart *gantt_chart;
    priority_queue<Process*, vector<Process*>, dynamic_priority_cmp>* dynamic_priority_queue;
    priority_queue<Process*, vector<Process*>, io_finish_cmp>* io_queue;
    vector<Process*>* starvation_queue;
   
  public:
    // CONSTRUCTORS
    HybridScheduler(int);
    ~HybridScheduler();

    // ACCESSORS
    int getTimeQuantum();
    int getProcessesScheduled();
    double getAverageWaitingTime();
    double getAverageTurnaroundTime();
    priority_queue<Process*, vector<Process*>, dynamic_priority_cmp>* getPriorityQueue();
    priority_queue<Process*, vector<Process*>, io_finish_cmp>* getIoQueue();
    vector<Process*>* getStarvationQueue();
    GanttChart *getGanttChart();

    // MUTATORS
    void addWaitingTime(int);
    void addTurnaroundTime(int);
    void incrementProcessesScheduled();

    // CONDITIONAL INDICATORS
    int allQueuesEmpty();

    // PROCESS ACCESS / MANAGMENT
    Process* getNextProcess();
    void demote(Process*, int);
    void promoteStarvedProcesses(int);
    void performIo(Process*, int);
    void checkForIoComplete(int);
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

  // create starvation queue (queue of all processes ordered by longest non-cpu time)
  starvation_queue = new vector<Process*>;

  // create io queue
  io_queue = new priority_queue<Process*, vector<Process*>, io_finish_cmp>;

  // create gantt chart
  gantt_chart = new GanttChart();
}


/*****************************************************
 | HybridScheduler - DECONSTRUCTOR                   |
 ****************************************************/
HybridScheduler::~HybridScheduler() {

  // free priority queue
  delete(dynamic_priority_queue);

  // free io queue
  delete(io_queue);

  // free starvation queue (queue of all processes ordered by longest non-cpu time)
  for (vector<Process*>::iterator iter = (*starvation_queue).begin(); iter < (*starvation_queue).end(); iter++) {
    Process *process = (*iter);
    iter = (*starvation_queue).erase(iter);
    iter--;
    delete(process);
  }
  delete(starvation_queue);

  // free gantt
  delete(gantt_chart);
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

vector<Process*>* HybridScheduler::getStarvationQueue() {
  return this->starvation_queue;
}

GanttChart *HybridScheduler::getGanttChart() {
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
  if ((*dynamic_priority_queue).size() > 0) {
    Process* process = (*dynamic_priority_queue).top();
    (*dynamic_priority_queue).pop();
    return process;
  }
  return NULL;
}

void HybridScheduler::demote(Process *process, int cpu_time) {
  (*process).setDynamicPriority((*process).getDynamicPriority() - cpu_time);

  // ensure process dynamic priority does not go below base priority
  if ((*process).getDynamicPriority() < (*process).getPriority()) {
    (*process).setDynamicPriority((*process).getPriority());
  }
  (*dynamic_priority_queue).push(process);
}

void HybridScheduler::promoteStarvedProcesses(int system_clock) {
  
  // vector to temporarily hold pop'd off starved processes
  make_heap((*starvation_queue).begin(), (*starvation_queue).end(), last_cpu_time_cmp());
  sort_heap((*starvation_queue).begin(), (*starvation_queue).end(), last_cpu_time_cmp());

  if (DEBUG){
    cout << "\nPROMOTE STARVED\n";
    printLastCpuTime((*starvation_queue));
  }

  // update starved processes
  if ((*starvation_queue).empty() == false) {
    for (vector<Process*>::iterator iter = (*starvation_queue).begin(); iter < (*starvation_queue).end() && system_clock - 100 >= (*(*iter)).getLastCpuTime(); iter++) {
      Process *process = (*iter);
      if ((*process).getBurstRemaining() == 0) {
        iter = (*starvation_queue).erase(iter);
        iter--;
        delete(process);
      } else {
        if ((*process).getPriority() < 50) {
          (*process).setDynamicPriority((*process).getDynamicPriority() + 10);

          // cap dynamic priority at 99
          if ((*process).getDynamicPriority() > 99) {
            (*process).setDynamicPriority(99);
          }
        }
      }
    }
  }

  priority_queue<Process*, vector<Process*>, dynamic_priority_cmp> *temp = this->dynamic_priority_queue;
  this->dynamic_priority_queue = new priority_queue<Process*, vector<Process*>, dynamic_priority_cmp>;

  if ((*temp).empty() == false) {
    while ((*temp).empty() == false) {
      (*dynamic_priority_queue).push((*temp).top());
      (*temp).pop();
    }
  }

  delete(temp);
}

void HybridScheduler::performIo(Process *process, int system_clock) {
  (*process).setIoFinish(system_clock + (*process).getIo());
  (*io_queue).push(process);
}

void HybridScheduler::checkForIoComplete(int system_clock) {
  if ((*io_queue).empty() == false) {
    while ((*io_queue).empty() == false && (*((*io_queue).top())).getIoFinish() == system_clock) {

      // increase dynamic priority after io
      Process *process = (*io_queue).top();
      if ((*process).getPriority() < 50) {
        (*process).setDynamicPriority((*process).getDynamicPriority() + (*process).getIo());

        // cap dynamic priority at 99
        if ((*process).getDynamicPriority() > 99) {
          (*process).setDynamicPriority(99);
        }
      }
      
      // add process back into ready queue
      (*dynamic_priority_queue).push(process);
      (*io_queue).pop();
    }
  }
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

  HybridScheduler *hybrid = new HybridScheduler (time_quantum);

  if (DEBUG) cout << "\n----------------------------\nSimulation Start:\n----------------------------\n";

  while ((*arrival_queue).empty() == false || (*hybrid).allQueuesEmpty() == false) {
    cpu_time = 0;

    // declare running process var
    Process* process;

    int process_set = 0;

    // a process is not chosen or ((the process is valid with more burst to complete) and the process has io and cpu time is less than time quantum -1 or cputime less than time quantum)
    while (!process_set || ((process != 0 && (*process).getBurstRemaining() > 0) && (((*process).getIo() > 0 && (cpu_time < (*hybrid).getTimeQuantum() - 1)) || (cpu_time < (*hybrid).getTimeQuantum())))) {


      // retrieve incoming processes
      checkForArrivalsHybrid(arrival_queue, (*hybrid).getPriorityQueue(), (*hybrid).getStarvationQueue(), system_clock);

      // retrieve incoming processes
      (*hybrid).checkForIoComplete(system_clock);

      // check for process promotions every 100 clock ticks
      if (system_clock % 100 == 0) {
        (*hybrid).promoteStarvedProcesses(system_clock);
      }

      // if no running process, select next process
      if (!process_set) {
        process = (*hybrid).getNextProcess();
        if (process != 0) {
          (*((*hybrid).getGanttChart())).start((*process).getP_ID(), system_clock, (*process).getUniqueID());
        }
        process_set = 1;
      }

      if (DEBUG){
        cout << "\n----------------------------\nArrival Queue:\n----------------------------\n";
        printArrival(*arrival_queue);
        cout << "\n----------------------------\nDynamic Pri Queue:\n----------------------------\n";
        printDynamicPriority(*((*hybrid).getPriorityQueue()));
        cout << "\n----------------------------\nIo Queue:\n----------------------------\n";
        printIoFinish(*((*hybrid).getIoQueue()));
        cout << "\n----------------------------\nStarvation Queue:\n----------------------------\n";
        printLastCpuTime(*((*hybrid).getStarvationQueue()));
        cout << "\n----------------------------\nCPU:\tClock: " << system_clock << "\tCpu time: " << cpu_time << "\n----------------------------\n";
        if (process != 0) cout << (*process).toStringCondensed();
      }

      // adjust process remaining_burst, last_cpu_time, cpu_time, and system_clock
      if (process != 0) {
        (*process).setBurstRemaining((*process).getBurstRemaining() - 1);
        (*process).setLastCpuTime(system_clock);
      }
      cpu_time++;
      system_clock++;
    }

    if (process != 0) {
      if ((*process).getBurstRemaining() > 0) {
        if ((*process).getIo() > 0) {
          (*hybrid).performIo(process, system_clock);
        } else {
          (*hybrid).demote(process, cpu_time);
        }
      } else {
        (*hybrid).incrementProcessesScheduled();
        (*hybrid).addWaitingTime(system_clock - (*process).getBurst() - (*process).getArrival());
        (*hybrid).addTurnaroundTime(system_clock - (*process).getArrival());
      }
      (*((*hybrid).getGanttChart())).end(system_clock);
    }
  }

  // print gantt chart
  (*((*hybrid).getGanttChart())).print();
  writeToFile("mfqs_output.txt", ((*hybrid).getGanttChart())); 

  // print statistics
  cout << "\n----------------------------\nSimulation Statistics:\n----------------------------\n";
  printf("Total Processes Scheduled: %d\n", (*hybrid).getProcessesScheduled());
  printf("Average Waiting Time: %.2f\n", (*hybrid).getAverageWaitingTime());
  printf("Average Turnaround Time: %.2f\n", (*hybrid).getAverageTurnaroundTime());
  
  if (DEBUG) cout << "\n----------------------------\nSimulation End\n----------------------------\n";

  delete(arrival_queue);
  delete(hybrid);
}

#endif

