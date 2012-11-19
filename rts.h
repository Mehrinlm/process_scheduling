#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <cstdlib>
#include "Process.h"

using namespace std;

int executeRTS(std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrivalQueue, bool softMode) {

  int numOfProcess = 0;
  unsigned long int turnaround = 0;
  unsigned long int waitTime = 0;
  

  //Setup Gantt Chart
  GanttChart* gantt_chart = new GanttChart();

  //Set system_clock to 0
  int system_clock = 0;
  
  Process* p;
  bool hardModeFailed = false;
  

  //Create queue (Priority)
  std::priority_queue<Process*, vector<Process*>, priority_cmp >* priorityQueue = new std::priority_queue<Process*, vector<Process*>, priority_cmp >;
  
  //Increases clock until all processes are done and both queueues empty
  while((*arrivalQueue).empty() == false || (*priorityQueue).empty() == false && !hardModeFailed){
    
    checkForArrivals(arrivalQueue, priorityQueue, system_clock);
    //Schedule!!!
    while ((*priorityQueue).empty() == false && !hardModeFailed){
      p = (*priorityQueue).top();                  //Get top of queue
      (*gantt_chart).start((*p).getP_ID(), system_clock, (*p).getUniqueID());    //Set up gantt chart
      int burst = (*p).getBurstRemaining();                 //Check remainint burst
      (*p).setBurstRemaining(--burst);                      //reduce burst by 1
      if (burst < 0 || system_clock > (*p).getDeadline()){
        //If that process is done, remove it from queue and free

        //add up stats info
        turnaround += (system_clock - (*p).getArrival());
        numOfProcess++;
        waitTime += ((system_clock - (*p).getArrival()) - ((*p).getBurst() - (*p).getBurstRemaining() -1));
        
        if (system_clock > (*p).getDeadline()) {
          if (softMode){
            (*gantt_chart).deadLineMissed();
          } else {
            hardModeFailed = true;
          } 
        }
          
        if (!hardModeFailed){
          (*priorityQueue).pop();
          free(p);
        }
        system_clock--;
      }
     
      //update clock and check for new arrivals
      system_clock++;
      (*gantt_chart).end(system_clock);
      checkForArrivals(arrivalQueue, priorityQueue, system_clock);
    }

    system_clock++;
  }
  if (hardModeFailed){
    cout << "Dealine not met on process P_ID: " << (*p).getP_ID() << " and in hard mode!" << endl;
  } else {
    (*gantt_chart).print();
    writeToFile("rts_output.txt", gantt_chart); 
  }
  free_queues(arrivalQueue, priorityQueue);
  delete(gantt_chart);
  if (numOfProcess != 0){
    printf("Average Turnaround: %.2f\n", (turnaround/double(numOfProcess)));
    printf("Average Wait: %.2f\n", (waitTime/double(numOfProcess)));
    printf("# of processes ran: %d\n", numOfProcess);
  }

  
}

