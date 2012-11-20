using namespace std;
#define DEBUG 0
#define TEST 0

#include <iostream>
#include <string>
#include <vector>
#include "io_utils.h"
#include "queue_utils.h"
#include "mfqs.h"
#include "hybrid.h"
#include "rts.h"






int main (int argc, char * arv[]) {
  // Create process arrical queue
  std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrival_queue = new std::priority_queue<Process*, vector<Process*>, arrive_cmp >();

  cout << "\nProcess Schedule Simulation\n----------------------------\n";

  // create process creation options
  vector<string> process_funcs;
  process_funcs.push_back("Add Process");
  process_funcs.push_back("Add Processes From File");
  process_funcs.push_back("View Processes");
  process_funcs.push_back("End Process Management");

  int manage_processes = 1;
  while (manage_processes) {
    // get process action
    int process_action = getUserChoice("Process Management", process_funcs);

    switch (process_action) {
      case 1: { // add process by hand
        Process* p = addProcess();
        if (p != NULL){
          (*arrival_queue).push(p);
        }
        break;
      }

      case 2: { // add processes from file
        string fileName = getUserString("Enter filename:");
        readProcessesFromFile(fileName.c_str(), arrival_queue);
        break;
      }

      case 3: { // view processes loaded
        printArrival(*arrival_queue);
        break;
      }

      case 4: { // finalize processes
        manage_processes = 0;
        break; }

      default: {
        addProcess();
        break;
      }
    }
  }

  // create scheduler options
  vector<string> schedulers;
  schedulers.push_back("MFQS");
  schedulers.push_back("Real-Time");
  schedulers.push_back("Hybrid");

  // get user-defined scheduler choice
  int scheduler = getUserChoice("Choose a scheduler:", schedulers);

  if (scheduler == 1) {
    int num_queues = getUserInt("Enter the number of queues (2-5)", 2, 5);
    int time_quantum = getUserInt("Enter the top queue's time quantum", 1, INT_MAX);
    int aging_time = getUserInt("Enter the aging time", 1, INT_MAX);
    executeMFQS(arrival_queue, num_queues, time_quantum, aging_time);
  } else if (scheduler == 2) {
    vector<string> rts;
    rts.push_back("Hard Mode");
    rts.push_back("Soft Mode");
    int rtsMode = getUserChoice("RTS Mode:", rts);
    executeRTS(arrival_queue, rtsMode == 2);
  } else if (scheduler == 3) {
    int time_quantum = getUserInt("Enter the scheduler time quantum", 1, INT_MAX);
    executeHybrid(arrival_queue, time_quantum);
  }

  return 0;
}

