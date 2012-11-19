using namespace std;
#define DEBUG 0
#define TEST 1

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include "io_utils.h"
#include "queue_utils.h"
#include "mfqs.h"
#include "hybrid.h"
#include "rts.h"

int main (int argc, char * arv[]) {
  // Create process arrical queue
  for (int h = 10; h < 40; h++) {
    ostringstream os;
    os << "./process_generator.pl process.txt 30 " << h << '\0';
    const char *command = (os.str()).c_str();
    system(command);

    ostringstream os1;
    os1 << "\n----------------------------\nMFQS\n----------------------------\n";
    os1 << "#QUE" << "\t" << "TQTM" << "\t" << "AGET" << "\t" << "PSCH" << "\t" << "AVGW" << "\t" << "AVGT" << "\t\n";
    writeToFile("tests.txt", (os1.str()).c_str());
    for (int i = 2; i < 6; i++) {
      int queues = i;
      for (int j = 1; j < 20; j++) {
        int time_quantum = j;
        for (int k = 1; k < 20; k++) {
          int aging_time = k;
          std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrival_queue = new std::priority_queue<Process*, vector<Process*>, arrive_cmp >();
          readProcessesFromFile("./process.txt", arrival_queue);
          executeMFQS(arrival_queue, queues, time_quantum, aging_time);
        }
      }
    }

    ostringstream os2;
    os2 << "\n----------------------------\nRTS\n----------------------------\n";
    os2 << "SOFT" << "\t" << "PSCH" << "\t" << "AVGW" << "\t" << "AVGT" << "\t\n";
    writeToFile("tests.txt", (os2.str()).c_str());
    for(int i = 0; i < 2; i++) {
      int mode = i;
      std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrival_queue = new std::priority_queue<Process*, vector<Process*>, arrive_cmp >();
      readProcessesFromFile("./process.txt", arrival_queue);
      executeRTS(arrival_queue, mode);
    }

    ostringstream os3;
    os2 << "\n----------------------------\nRTS\n----------------------------\n";
    os3 << "\n----------------------------\nHybrid\n----------------------------\n";
    os3 << "TQTM" << "\t" << "PSCH" << "\t" << "AVGW" << "\t" << "AVGT" << "\t\n";
    writeToFile("tests.txt", (os3.str()).c_str());
    for (int i = 0; i < 20; i++) {
      int time_quantum = i;
      std::priority_queue<Process*, vector<Process*>, arrive_cmp >* arrival_queue = new std::priority_queue<Process*, vector<Process*>, arrive_cmp >();
      readProcessesFromFile("./process.txt", arrival_queue);
      executeHybrid(arrival_queue, time_quantum);
    }
  }

  return 0;
}

