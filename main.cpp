#include <getopt.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "hash.hpp"
using namespace std;

string outputHelp() {
  stringstream res;
  res << "pass -f and a filename to read from a file\n"
      << "pass -s to read from stdin\n"
      << "pass -n to and a filename and number of lines as an argument. Format "
         "is \"2 file.txt\" (dont forget quotes)\n";
  return res.str();
}

inline string readFromFile(const string& path) {
  ostringstream buf;
  ifstream file(path);
  if (file.good()) {
    buf << file.rdbuf();
    file.close();
    // removes the newline at the end of the file
    if (!buf.str().empty() && buf.str()[buf.str().length() - 1] == '\n') {
      return buf.str().substr(0, buf.str().size() - 1);
    } else {
      return buf.str();
    }
  } else {
    throw "file " + path + " cannot be opened\n";
  }
}

string callHashingFunctionFile(const string& path) {
  Hash h;
  return h.HashingFunction(readFromFile(path));
}

string callHashingFunctionString(const string& message) {
  Hash h;
  return h.HashingFunction(message);
}

void callHashingFunctionNStrings(const string& arg) {
  istringstream argBuf(arg);
  size_t num;
  string path;

  // parse arguments
  // TODO: check whether the input is valid
  argBuf >> num;
  argBuf >> path;
  istringstream message(readFromFile(path));

  // hash every line
  for (size_t i = 0; i < num; i++) {
    Hash h;
    string line;
    getline(message, line);
    cout << h.HashingFunction(line) << endl;
  }
}

int main(int argc, char* argv[]) {
  int c, index;

  if (argc == 1) {
    cout << outputHelp();
    return 42;
  }

  while ((c = getopt(argc, argv, "f:n:s:h")) != -1) {
    switch (c) {
      case 'f':
        try {
          cout << callHashingFunctionFile(optarg) << endl;
          break;
        } catch (const string& err) {
          cerr << err;
          return EXIT_FAILURE;
        }
      case 'n':
        try {
          callHashingFunctionNStrings(optarg);
          break;
        } catch (const string& err) {
          cerr << err;
          return EXIT_FAILURE;
        }
      case 's':
        cout << optarg << endl;
        cout << callHashingFunctionString(optarg) << endl;
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
