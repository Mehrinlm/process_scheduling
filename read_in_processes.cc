#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Process.h"

using namespace std;


vector<Process> readInProcess(const char *fileName){
  // initialize processes vector
  vector<Process> processes;

  ifstream in;
  string line;
  in.open(fileName);
  if (in.is_open()){
    while (in.good()){
      getline(in, line);

      Process process;
      process.create(line);
      processes.push_back(process);
      string output = process.toString();

      cout << output << endl;
    }
    in.close();
  }

  return processes;
}

