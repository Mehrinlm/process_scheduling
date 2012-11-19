#include <iostream>

#ifndef GANTT
#define GANTT

struct gantt_item {
  int p_id;
  int start;
  int end;
  bool metDeadline;
};

class GanttChart {
    vector<gantt_item*> *gantt_items;
   
  public:
    GanttChart();
    ~GanttChart();
    void start(int, int);
    void end(int);
    void print();
    void deadLineMissed();
};

GanttChart::GanttChart(){
  this->gantt_items = new vector<gantt_item*>;
}

GanttChart::~GanttChart(){
  for(vector<gantt_item*>::iterator iter = (*gantt_items).begin(); iter < (*gantt_items).end(); iter++) {
    struct gantt_item *next_gantt = (*iter);
    iter = (*gantt_items).erase(iter);
    iter--;
    free(next_gantt);
  }
  delete(gantt_items);
}

void GanttChart::start(int p_id, int system_clock) {
  // only add new gantt_item if it has a different p_id
  if ((*gantt_items).empty() || ((*gantt_items)[(*gantt_items).size() - 1]->p_id != p_id || (*gantt_items)[(*gantt_items).size() - 1]->end != system_clock)) {
    struct gantt_item *next_gantt = (struct gantt_item *) malloc(sizeof(struct gantt_item));
    next_gantt->p_id = p_id;
    next_gantt->metDeadline = true;
    next_gantt->start = system_clock;
    next_gantt->end = system_clock;
    (*gantt_items).push_back(next_gantt);
  }
}

void GanttChart::deadLineMissed(){
  ((*gantt_items)[(*gantt_items).size() - 1])->metDeadline = false;
}

void GanttChart::end(int system_clock) {
  ((*gantt_items)[(*gantt_items).size() - 1])->end = system_clock;
}

void GanttChart::print() {

  //Get the largestEnd value
  int totalEnd = 0;
  for (int i = 0; i < (*gantt_items).size(); i++) {
    if (totalEnd < (*gantt_items)[i]->end){
       totalEnd = (*gantt_items)[i]->end;
    }
  }
  
  
  cout << "\n----------------------------\nGantt Chart\n----------------------------\n";
  for (int i = 0; i < (*gantt_items).size(); i++) {
    ostringstream convert;
    convert << "P_ID: ";
    convert << (*gantt_items)[i]->p_id;
    while (convert.str().size() < 15) convert << " ";
    convert << "Start: ";
    convert << (*gantt_items)[i]->start;
    while (convert.str().size() < 30) convert << " ";
    convert << "End: ";
    convert << (*gantt_items)[i]->end;
    
    if ((*gantt_items)[i]->metDeadline == false){
      while (convert.str().size() < 40) convert << " ";
      convert << "DEADLINE FAILED";
    }
    cout << convert.str() << endl;
  }
}

#endif

