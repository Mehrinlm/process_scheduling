#include <iostream>
#include <string>
#include <vector>
#include "read_in_processes.cc"
#include "io_utils.h"
#include "mfqs.h"

using namespace std;

// global process list
vector<Process> processes;



// -------------- START UNIMPLEMENTED FUNC STUBS -------------- //

// execute scheduler funcs
int executeMFQS();
int executeRTS(){cout << "RTS exec";};
int executeHybrid(){cout << "Hybrid exec";};

// process funcs
void addProcess(){cout << "Add Process";};

// -------------- END UNIMPLEMENTED FUNC STUBS -------------- //



void printAllProcesses();


int main (int argc, char * arv[]) {

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
        processes = readInProcess(fileName.c_str());
        break;
      }

      case 3: { // view processes loaded
        printAllProcesses();
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
    executeMFQS();
  } else if (scheduler == 2) {
    executeRTS();
  } else if (scheduler == 3) {
    executeHybrid();
  }

  return 0;
}

void printAllProcesses(){
  for (int i = 0; i < processes.size(); i++) {
    cout << processes[i].toString() << endl;
  }
}

int executeMFQS() {
  cout << "\nMulti-level Feedback Queue\n----------------------------\n";

  int num_queues = getUserInt("Enter the number of queues (1-5)", 1, 5);
  int time_quantum = getUserInt("Enter the top queue's time quantum", 1, INT_MAX);
  int aging_time = getUserInt("Enter the aging time", 1, INT_MAX);

  Mfqs mfqs (num_queues, time_quantum, aging_time);
}
