#ifndef TRAIN_H
#define TRAIN_H

#include <iostream>
#include <vector>

#include "Network.h"

class Train
{
private:
  std::string getWeightsDir(char* argvZero);
  void printHelp();
  void isValInt(char* opt, char* val);
  std::vector<std::string> parseOptions(int argc, char* argv[]);

public:
  int main(int argc, char* argv[]);
};

#endif
