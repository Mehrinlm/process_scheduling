#include <iostream>
#include <fstream>
#include <string>
#include "Process.h"

using namespace std;


void readInProcess(char* fileName){
  ifstream in;
  string line;
  in.open(fileName);
  if (in.is_open()){
    while (in.good()){
      getline(in, line);
      Process pro;
      pro.create(line);
      string output = pro.toString();
      cout << output << endl;
    }
    in.close();
  }

}

int main (int argc, char *argv[]) {
  readInProcess(argv[1]);
}