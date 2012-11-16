#include <iostream>
#include <string>
#include <limits>
#include <climits>

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

