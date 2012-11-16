#include <sstream>

#ifndef PROCESS
#define PROCESS

using namespace std;
class Process {
   int P_ID;
   int burst;
   int arrival;
   int priority;
   int deadline;
   int io;
   int burstRemaining;
   
  public:
   int create(string info);
   string toString();
   int getP_ID();
   int getPriority();
   int getArrival();
   int getBurstRemaining();
   void setBurstRemaining(int burstReamin);
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

int Process::getP_ID(){
  return P_ID;
}

int Process::getPriority(){
  return priority;
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
      convert >> varaibles[index++];
    }
  }
  string num = info.substr(start);
  istringstream convert(num);
  convert >> varaibles[index];

  P_ID = varaibles[0];
  burst = varaibles[1];
  arrival = varaibles[2];
  priority = varaibles[3];
  deadline = varaibles[4];
  io = varaibles[5];
  burstRemaining = burst;

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
#endif
