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
      if ((*process).create(line)){             //calls this method to parse out input tring
        (*queue).push(process);               //Adds process to priority_que
      } else {
        delete(process);
      }
    }
    in.close();
  }
}

// Prints out all processes in order of arrival
void printArrival(std::priority_queue<Process*, vector<Process*>, arrive_cmp > queue){
  while(queue.empty() == false){
    Process* p =queue.top();
    string output = (*p).toStringCondensed();
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

// Prints out all processes in order of last cpu time
void printLastCpuTime(std::vector<Process*> queue){
  for (vector<Process*>::iterator iter = queue.begin(); iter < queue.end(); iter++) {
    Process* p = (*iter);
    string output = (*p).toStringCondensed();
    cout << output << endl;
  }
}

// Prints out all processes in order of dynamic priority
void printDynamicPriority(std::priority_queue<Process*, vector<Process*>, dynamic_priority_cmp > queue){
  while(queue.empty() == false){
    Process* p =queue.top();
    string output = (*p).toStringCondensed();
    cout << output << endl;
    queue.pop();
  }
}

// Prints out all processes in order of io finish
void printIoFinish(std::priority_queue<Process*, vector<Process*>, io_finish_cmp > queue){
  while(queue.empty() == false){
    Process* p =queue.top();
    string output = (*p).toStringCondensed();
    cout << output << endl;
    queue.pop();
  }
}

// Frees all processesin queues
void free_queues(std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrivalQueue,
                  std::priority_queue<Process*, vector<Process*>, priority_cmp >* priorityQueue){
    while ((*arrivalQueue).size() > 0){
      Process *p = (*arrivalQueue).top();
      (*arrivalQueue).pop();
      free(p);
    }    

    while ((*priorityQueue).size() > 0){
      Process *p = (*priorityQueue).top();
      (*priorityQueue).pop();
      free(p);
    }   
    
    delete(arrivalQueue);
    delete(priorityQueue);
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
                        std::priority_queue<Process*, vector<Process*>, queue_arrive_cmp >* priorityQueue,
                        int system_clock){
  if ((*arrivalQueue).empty() == false){
    while((*arrivalQueue).empty() == false && (*((*arrivalQueue).top())).getArrival() == system_clock){
      (*((*arrivalQueue).top())).setQueueArrival(system_clock);
      (*priorityQueue).push((*arrivalQueue).top());
      (*arrivalQueue).pop();
    }
  }
}

void checkForArrivalsHybrid(std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrivalQueue,
                        std::priority_queue<Process*, vector<Process*>, dynamic_priority_cmp >* priorityQueue,
                        std::vector<Process*>* starvationQueue,
                        int system_clock){
  if ((*arrivalQueue).empty() == false){
    while((*arrivalQueue).empty() == false && (*((*arrivalQueue).top())).getArrival() == system_clock){
      (*priorityQueue).push((*arrivalQueue).top());
      (*starvationQueue).push_back((*arrivalQueue).top());
      (*arrivalQueue).pop();
    }
  }
}

