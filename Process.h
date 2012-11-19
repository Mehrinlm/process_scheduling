#include <sstream>

#ifndef PROCESS
#define PROCESS

using namespace std;
class Process {
   int P_ID;
   int burst;
   int arrival;
   int priority;
   int dynamic_priority;    // used in hybrid priority queue
   int deadline;
   int io;
   int burstRemaining;
   int queue_arrival;       // used in mfqs queues
   int io_finish;           // used in hybrid io queue
   int last_cpu_time;       // used in hybrid io queue
   
  public:
   int create(string info);
   string toString();
   string toStringCondensed();
   int getP_ID();
   int getIo();
   int getDeadline();
   int getPriority();
   int getDynamicPriority();
   void setDynamicPriority(int);
   int getArrival();
   int getQueueArrival();
   void setQueueArrival(int);
   int getBurst();
   int getBurstRemaining();
   void setBurstRemaining(int);
   int getIoFinish();
   void setIoFinish(int);
   int getLastCpuTime();
   void setLastCpuTime(int);
};

int Process::getBurstRemaining(){
  return burstRemaining;
}

void Process::setBurstRemaining(int burstRemain){
  burstRemaining = burstRemain;
}

int Process::getArrival(){
  return arrival;
}

int Process::getQueueArrival(){
  return queue_arrival;
}

void Process::setQueueArrival(int system_clock){
  this->queue_arrival = system_clock;
}

int Process::getBurst(){
  return arrival;
}

int Process::getDeadline(){
  return deadline;
}

int Process::getP_ID(){
  return P_ID;
}

int Process::getIo(){
  return io;
}

int Process::getPriority(){
  return priority;
}

int Process::getDynamicPriority(){
  return dynamic_priority;
}

void Process::setDynamicPriority(int dynamic_priority){
  this->dynamic_priority = dynamic_priority;
}

int Process::getIoFinish(){
  return this->io_finish;
}

void Process::setIoFinish(int io_finish) {
  this->io_finish = io_finish;
}

int Process::getLastCpuTime(){
  return this->last_cpu_time;
}

void Process::setLastCpuTime(int last_cpu_time) {
  this->last_cpu_time = last_cpu_time;
}

//compare by priority -- P_ID is tie breaker
struct priority_cmp
    : public binary_function<Process, Process, bool> {  
        bool operator()(Process* left, Process* right) const{
            int i = (*left).getPriority() - (*right).getPriority();
            if (i == 0){
              i = (*left).getP_ID() - (*right).getP_ID();
            }
            return i > 0;
        }
};

//compare by dynamic_priority -- P_ID is tie breaker (used by hybrid scheduler main priority queue)
struct dynamic_priority_cmp
    : public binary_function<Process, Process, bool> {  
        bool operator()(Process* left, Process* right) const{
            int i = (*right).getDynamicPriority() - (*left).getDynamicPriority();
            if (i == 0){
              i = (*left).getP_ID() - (*right).getP_ID();
            }
            return i > 0;
        }
};

//compare by io finish_time -- P_ID is tie breaker (used by hybrid scheduler io queue)
struct io_finish_cmp
    : public binary_function<Process, Process, bool> {  
        bool operator()(Process* left, Process* right) const{
            int i = (*left).getIoFinish() - (*right).getIoFinish();
            if (i == 0){
              i = (*left).getP_ID() - (*right).getP_ID();
            }
            return i > 0;
        }
};

//compare by last cpu time -- P_ID is tie breaker (used by hybrid scheduler io queue)
struct last_cpu_time_cmp
    : public binary_function<Process, Process, bool> {  
        bool operator()(Process* left, Process* right) const{
            int i = (*right).getLastCpuTime() - (*left).getLastCpuTime();
            if (i == 0){
              i = (*left).getP_ID() - (*right).getP_ID();
            }
            return i > 0;
        }
};

//Compare by time, that way we can pop them as they arrive
struct arrive_cmp
    : public binary_function<Process, Process, bool> {  
        bool operator()(Process* left, Process* right) const{
            int i = (*left).getArrival() - (*right).getArrival();
            if (i == 0){
              i = (*left).getP_ID() - (*right).getP_ID();
            }
            return i > 0;
        }
};

//Compare by queue arrival time (used by all mfqs queues)
struct queue_arrive_cmp
    : public binary_function<Process, Process, bool> {  
        bool operator()(Process* left, Process* right) const{
            int i = (*left).getQueueArrival() - (*right).getQueueArrival();
            if (i == 0){
              i = (*left).getP_ID() - (*right).getP_ID();
            }
            return i > 0;
        }
};

int boxInt(int val, int min, int max) {
  if (val < min) {
    return min;
  } else if (val > max) {
    return max;
  } else {
    return val;
  }
}

int Process::create(string info){
  int varaibles[6];
  int index = 0;
  int start = 0;
  int i;
  for (i = 0; i < info.size(); i++){
    if (info[i] == '\t'){
      string num = info.substr(start, i);
      start = i+1;
      istringstream convert(num);
      convert >> varaibles[index];
      if (varaibles[index++] < 0){
        return 0;
      }
    }
  }
  string num = info.substr(start);
  istringstream convert(num);
  convert >> varaibles[index];

  P_ID = boxInt(varaibles[0], 0, INT_MAX);
  burst = boxInt(varaibles[1], 0, INT_MAX);
  burstRemaining = boxInt(varaibles[1], 0, INT_MAX);
  arrival = boxInt(varaibles[2], 0, INT_MAX);
  priority = boxInt(varaibles[3], 0, 99);
  dynamic_priority = boxInt(varaibles[3], 0, 99);
  deadline = boxInt(varaibles[4], 0, INT_MAX);
  io = boxInt(varaibles[5], 0, INT_MAX);
  io_finish = 0;
  last_cpu_time = 0;
  
  if (burst == 0)
    return 0;
  
  return 1;
}

string Process::toString(){
  ostringstream convert;

  convert << "Process: ";
  convert << "\n\t P_ID: ";
  convert << P_ID;
  
  convert << "\n\t burst: ";
  convert << burst;
  
  convert << "\n\t arrival: ";
  convert << arrival;
  
  convert << "\n\t priority:";
  convert << priority;

  convert << "\n\t deadline: ";
  convert << deadline;
  
  convert << "\n\t io: ";
  convert << io;

  return convert.str();
}

string Process::toStringCondensed(){
  ostringstream convert;

  convert << "P_ID: ";
  convert << P_ID;
  convert << " ";
  
  convert << "\t burst left: ";
  convert << burstRemaining;
  
  convert << "\t arrival: ";
  convert << arrival;
  
  convert << "\t priority:";
  convert << priority;

  convert << "\t Deadline:";
  convert << deadline;
  
  convert << "\t dy pri:";
  convert << dynamic_priority;

  convert << "\t io: ";
  convert << io;

  convert << "\t last cpu: ";
  convert << last_cpu_time;

  convert << "\t io finish: ";
  convert << io_finish;

  return convert.str();
}
#endif
