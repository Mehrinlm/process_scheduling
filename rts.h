#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <cstdlib>
#include "Process.h"

using namespace std;

int system_clock;

//Checks arrivalqueue to see if any entered, if so, add to priorityQueue
void checkForArrivals(std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrivalQueue,
                        std::priority_queue<Process*, vector<Process*>, priority_cmp >* priorityQueue){
  if ((*arrivalQueue).empty() == false){
    while((*arrivalQueue).empty() == false && (*((*arrivalQueue).top())).getArrival() == system_clock){
      (*priorityQueue).push((*arrivalQueue).top());
      (*arrivalQueue).pop();
    }
  }
}

int executeRTS(std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrivalQueue) {

  //Set system_clock to 0
  system_clock = 0;
  
  printArrival(*arrivalQueue);

  //Create queue (Priority)
  std::priority_queue<Process*, vector<Process*>, priority_cmp >* priorityQueue = new std::priority_queue<Process*, vector<Process*>, priority_cmp >;
  
  //Increases clock until all processes are done and both queueues empty
  while((*arrivalQueue).empty() == false || (*priorityQueue).empty() == false){
    
    checkForArrivals(arrivalQueue, priorityQueue);
    //Schedule!!!
    while ((*priorityQueue).empty() == false){
      Process* p = (*priorityQueue).top();   //Get top of que
      int burst = (*p).getBurstRemaining(); //Check remainint burst
      (*p).setBurstRemaining(--burst);  //reduce burst by 1
      if (DEBUG) cout << "clock: " << system_clock << "\tsize: " << (*priorityQueue).size() << "\ttimeLeft: " << burst << endl;
      if (burst < 0){
        //If that process is done, remove it from queue and free
        (*priorityQueue).pop();
        free(p);
      }
      //update clock and check for new arrivals
      system_clock++;
      checkForArrivals(arrivalQueue, priorityQueue);
    }

    system_clock++;
  }
}

