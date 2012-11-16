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
   
  public:
   int create(string info);
   string toString();
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
