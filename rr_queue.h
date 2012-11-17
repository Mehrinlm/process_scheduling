#ifndef RR_QUEUE
#define RR_QUEUE

using namespace std;
class RR_Queue {
    int time_quantum;
    priority_queue<Process*, vector<Process*>, arrive_cmp>* queue;
   
  public:
    RR_Queue(int);
    int getTime_quantum();
    priority_queue<Process*, vector<Process*>, arrive_cmp>* getQueue();
};

RR_Queue::RR_Queue(int time_quantum_in) {

  // initialize priority queue based on arrival
  queue = new priority_queue<Process*, vector<Process*>, arrive_cmp>;

  cout << "Constructing RR_Queue\n";
  time_quantum = time_quantum_in;
}

int RR_Queue::getTime_quantum() {
  return time_quantum;
}

priority_queue<Process*, vector<Process*>, arrive_cmp>* RR_Queue::getQueue() {
  return queue;
}

#endif
