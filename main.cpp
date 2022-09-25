#include <iostream>
#include "hash.hpp"

using namespace std;

int main() {
  string message;
  getline(std::cin, message);
  Hash h;
  cout << h.HashingFunction(message) << endl;
  return 0;
}
