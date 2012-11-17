#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <cstdlib>
#include "Process.h"

void readProcessesFromFile(const char* fileName, priority_queue<Process*, vector<Process*>, arrive_cmp >* queue){
  ifstream in;
  string line;
  in.open(fileName);
  
  //Get all lines of file
  if (in.is_open()){
    while (in.good()){
      getline(in, line);
      Process* process = new Process();     //Create Process
      (*process).create(line);              //calls this method to parse out input tring
      (*queue).push(process);               //Adds process to priority_que
    }
    in.close();
  }
}

// Prints out all processes in order of arrival
void printArrival(std::priority_queue<Process*, vector<Process*>, arrive_cmp > queue){
  while(queue.empty() == false){
    Process* p =queue.top();
    string output = (*p).toString();
    cout << output << endl;
    queue.pop();
  }
}

// Prints out all processes in order of priority
void printPriority(std::priority_queue<Process*, vector<Process*>, priority_cmp > queue){
  while(queue.empty() == false){
    Process* p =queue.top();
    string output = (*p).toString();
    cout << output << endl;
    queue.pop();
  }
}

/*****************************************************
 | Check For Arriving Processes                      |
 | --------------------------------------------------|
 | checks arrival_queue for incoming processes       |
 | if found, added to priority_queue                 |
 |                                                   |
 ****************************************************/
void checkForArrivals(std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrivalQueue,
                        std::priority_queue<Process*, vector<Process*>, priority_cmp >* priorityQueue,
                        int system_clock){
  if ((*arrivalQueue).empty() == false){
    while((*arrivalQueue).empty() == false && (*((*arrivalQueue).top())).getArrival() == system_clock){
      (*priorityQueue).push((*arrivalQueue).top());
      (*arrivalQueue).pop();
    }
  }
}

void checkForArrivalsMFQS(std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrivalQueue,
                        std::priority_queue<Process*, vector<Process*>, arrive_cmp >* priorityQueue,
                        int system_clock){
  if ((*arrivalQueue).empty() == false){
    while((*arrivalQueue).empty() == false && (*((*arrivalQueue).top())).getArrival() == system_clock){
      (*priorityQueue).push((*arrivalQueue).top());
      (*arrivalQueue).pop();
    }
  }
}
