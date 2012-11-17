#include <iostream>
#include "rr_queue.h"
#include "fcfs_queue.h"

#ifndef MFQS
#define MFQS

using namespace std;
class Mfqs {
    vector<RR_Queue> rr_queues;
    FCFS_Queue fcfs_queue;
    int aging_time;
   
  public:
    Mfqs(int, int, int);
    vector<RR_Queue> getRR_queues();
    FCFS_Queue getFCFS_queue();
};

Mfqs::Mfqs(int num_queues, int time_quantum, int aging_time) {
  cout << "Constructing MFQS\n";
  for (int i = 0; i < num_queues - 1; i++) {
    rr_queues.push_back(RR_Queue(time_quantum));
    time_quantum *= 2;
  }
  
  cout << "\n\nRR_Queues Created:\n";
  for (int i = 1; i < rr_queues.size() + 1; i++) {
    cout << "Queue #" << i <<  "\tQuantum: " << rr_queues[i - 1].getTime_quantum() << "\n";
  }
}

vector<RR_Queue> Mfqs::getRR_queues() {
  return this->rr_queues;
}

FCFS_Queue Mfqs::getFCFS_queue() {
  return this->fcfs_queue;
}

int executeMFQS() {
  cout << "\nMulti-level Feedback Queue\n----------------------------\n";

  int num_queues = getUserInt("Enter the number of queues (1-5)", 1, 5);
  int time_quantum = getUserInt("Enter the top queue's time quantum", 1, INT_MAX);
  int aging_time = getUserInt("Enter the aging time", 1, INT_MAX);

  Mfqs mfqs (num_queues, time_quantum, aging_time);
}

#endif
