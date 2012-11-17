using namespace std;
#define DEBUG 1

#include <iostream>
#include <string>
#include <vector>
#include "io_utils.h"
#include "queue_utils.h"
#include "mfqs.h"
#include "rts.h"


// -------------- START UNIMPLEMENTED FUNC STUBS -------------- //

// execute scheduler funcs
int executeHybrid(){cout << "Hybrid exec";};

// process funcs
void addProcess(){cout << "Add Process";};

// -------------- END UNIMPLEMENTED FUNC STUBS -------------- //



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
        addProcess();
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
    executeMFQS(arrival_queue);
  } else if (scheduler == 2) {
    executeRTS(arrival_queue);
  } else if (scheduler == 3) {
    executeHybrid();
  }

  return 0;
}

