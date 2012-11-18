#include <iostream>

#ifndef GANTT
#define GANTT

struct gantt_item {
  int p_id;
  int start;
  int end;
};

class GanttChart {
    vector<gantt_item*> *gantt_items;
   
  public:
    GanttChart();
    void start(int, int);
    void end(int);
    void print();
};

GanttChart::GanttChart(){
  this->gantt_items = new vector<gantt_item*>;
}

void GanttChart::start(int p_id, int system_clock) {
  // only add new gantt_item if it has a different p_id
  if ((*gantt_items).empty() || (*gantt_items)[(*gantt_items).size() - 1]->p_id != p_id) {
    struct gantt_item *next_gantt = (struct gantt_item *) malloc(sizeof(struct gantt_item));
    next_gantt->p_id = p_id;
    next_gantt->start = system_clock;
    next_gantt->end = system_clock;
    (*gantt_items).push_back(next_gantt);
  }
}

void GanttChart::end(int system_clock) {
  ((*gantt_items)[(*gantt_items).size() - 1])->end = system_clock;
}

void GanttChart::print() {
  cout << "\n----------------------------\nGantt Chart\n----------------------------\n";
  for (int i = 0; i < (*gantt_items).size(); i++) {
    printf("P_ID: %d\tStart: %d\tEnd:%d\n", (*gantt_items)[i]->p_id, (*gantt_items)[i]->start, (*gantt_items)[i]->end);
  }
}

#endif

