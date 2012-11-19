#include <iostream>
#include <limits>
#include <climits>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include<ctime>
#include "gantt.h"
#include "Process.h"


string getUserString(string message) {

  cout << message + "\n";

  string input = "";
  cin >> input;

  // clear iostream
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  return input;
}

int getUserChoice(string message, vector<string> options) {

  cout << message + "\n";
  for (int i = 1; i < options.size() + 1; i++) {
    cout << i << "\t" + options[i - 1] + "\n";
  }

  int choice = 0;
  while (choice < 1 || choice > options.size()) {
    cout << "Choice:";
    cin >> choice;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Not a valid choice\n";
    }
  }

  // clear iostream
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  return choice;
}

//Adds a process by promting user for inputs
Process* addProcess(){
  string input = "";
  
  //P_ID
  input += getUserString("Enter P_ID: ")+ "\t";
  
  //Burst
  input += getUserString("Enter Burst: ")+ "\t";
  
  //Arrival
  input += getUserString("Enter Arrival: ")+ "\t";
  
  //Priority
  input += getUserString("Enter Priority: ")+ "\t";
  
  //Deadline
  input += getUserString("Enter Deadline: ")+ "\t";
  
  //io
  input += getUserString("Enter I/O: ");
  
  Process* process = new Process(); 
  if ((*process).create(input)){
    return process;
  } else {
    delete(process);
    return NULL;
  }
}

int getUserInt(string message, int min, int max) {

  int value = INT_MIN;
  while (value < min || value > max) {
    cout << message + ":\n";
    cin >> value;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Not a valid integer\n";
    }
  }

  // clear iostream
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  return value;
}

//Writes gantt chart data to file
int writeToFile(const char* fileName, GanttChart* chart){
  ofstream out;
  
  out.open(fileName);
  
  streambuf *psbuf, *backup;

  backup = cout.rdbuf();     // back up cout's streambuf

  psbuf = out.rdbuf();   // get file's streambuf
  cout.rdbuf(psbuf);         // assign streambuf to cout
  
  (*chart).printParseable();
  
  cout.rdbuf(backup);        // restore cout's original streambuf

  out.close();
}
  
  
  

