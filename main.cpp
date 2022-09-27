#include <getopt.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include "hash.hpp"
using namespace std;

string outputHelp() {
  stringstream res;
  res << "pass -f and a filename to read from a file\n"
      << "pass -s to read from stdin\n";
  return res.str();
}

inline string readFromFile(const string& path) {
  ostringstream buf;
  ifstream file(path);
  buf << file.rdbuf();
  file.close();
  // removes the newline at the end of the file
  return buf.str().substr(0, buf.str().size() - 1);
}

int main(int argc, char* argv[]) {
  int c, index;
  string message;
  Hash h;

  if (argc == 1) {
    cout << outputHelp();
    return 42;
  }

  while ((c = getopt(argc, argv, "f:sh")) != -1) {
    switch (c) {
      case 'f':
        cerr << "reading from file " << optarg << endl;
        cout << h.HashingFunction(readFromFile(optarg)) << endl;
        break;
      case 's':
        cerr << "working with a string\n";
        getline(std::cin, message);
        cout << h.HashingFunction(message) << endl;
        break;
      case 'h':
        cout << outputHelp();
        break;
      case '?':
        return 42;
      default:
        abort();
    }
  }

  for (index = optind; index < argc; index++)
    printf("Non-option argument %s\n", argv[index]);

  return 0;
}
