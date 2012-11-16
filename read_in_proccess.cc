#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <cstdlib>
#include "Process.h"

#DEFINE DEBUG 1

using namespace std;

int system_clock;

void readInProcess(char* fileName, priority_queue<Process*, vector<Process*>, arrive_cmp >* que){
  ifstream in;
  string line;
  //Open file
  in.open(fileName);
  
  //Get all lines of flies
  if (in.is_open()){
    while (in.good()){
      getline(in, line);
      Process* pro = new Process(); //Create Process
      (*pro).create(line);          //calls this method to parse out input tring
      (*que).push(pro);             //Adds process to priority_que
    }
    in.close();
  }

}

//Print out que DEBUG REASONS
void printArrival(std::priority_queue<Process*, vector<Process*>, arrive_cmp > que){
  //Print out que
  
  while(que.empty() == false){
    Process* p =que.top();
    string output = (*p).toString();
    cout << output << endl;
    que.pop();
  }
}
//Print out que DEBUG REASONS
void printPriority(std::priority_queue<Process*, vector<Process*>, priority_cmp > que){
  //Print out que
  
  while(que.empty() == false){
    Process* p =que.top();
    string output = (*p).toString();
    cout << output << endl;
    que.pop();
  }
}

//Checks arrivalQue to see if any entered, if so, add to priorityQue
void checkForArrivals(std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrivalQue,
                        std::priority_queue<Process*, vector<Process*>, priority_cmp >* priorityQue){
  if ((*arrivalQue).empty() == false){
    while((*arrivalQue).empty() == false && (*((*arrivalQue).top())).getArrival() == system_clock){
      (*priorityQue).push((*arrivalQue).top());
      (*arrivalQue).pop();
    }
  }
}

int main (int argc, char *argv[]) {

  //Set system_clock to 0
  system_clock = 0;
  
  //Create que (Arrival)
  std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrivalQue = new std::priority_queue<Process*, vector<Process*>, arrive_cmp >();
  
  //Populte que from text file
  readInProcess(argv[1], arrivalQue);

  printArrival(*arrivalQue);
  //Create que (Priority)
  std::priority_queue<Process*, vector<Process*>, priority_cmp >* priorityQue = new std::priority_queue<Process*, vector<Process*>, priority_cmp >;
  
  //Increases clock until all processes are done and both queues empty
  while((*arrivalQue).empty() == false || (*priorityQue).empty() == false){
    
    checkForArrivals(arrivalQue, priorityQue);
    //Schedule!!!
    while ((*priorityQue).empty() == false){
      Process* p = (*priorityQue).top();   //Get top of que
      int burst = (*p).getBurstRemaining(); //Check remainint burst
      (*p).setBurstRemaining(--burst);  //reduce burst by 1
      if (DEBUG) cout << "clock: " << system_clock << "\tsize: " << (*priorityQue).size() << "\ttimeLeft: " << burst << endl;
      if (burst < 0){
        //If that process is done, remove it from que and free
        (*priorityQue).pop();
        free(p);
      }
      //update clock and check for new arrivals
      system_clock++;
      checkForArrivals(arrivalQue, priorityQue);
    }

    system_clock++;
  }
 // printQue(*arrivalQue);
 // cout << "---------------------------" << endl;
 // printQue(*arrivalQue);
}

