#ifndef RR_QUEUE
#define RR_QUEUE

using namespace std;
class RR_Queue {
    int time_quantum;
    vector<Process> processes;
   
  public:
    RR_Queue(int);
    int getTime_quantum();
};

RR_Queue::RR_Queue(int time_quantum_in) {
  cout << "Constructing RR_Queue\n";
  time_quantum = time_quantum_in;
}

int RR_Queue::getTime_quantum() {
  return time_quantum;
}

#endif
