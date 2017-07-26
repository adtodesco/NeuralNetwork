#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream> 

#include "Constants.h"

class OptionParser
{
private:
  void error(int errorCode, std::string info = "");
  void printHelp();
  void testNodes(std::unordered_map<int, std::string> options);
  std::string testInt(std::string val);
  bool fileExist(std::string val);

  std::unordered_map<int, std::string> getTestOptions();
  std::unordered_map<int, std::string> getTrainOptions();

  int cmdtype;
  std::vector<std::string> arguments;
public:
  OptionParser(std::vector<std::string> args, int type);

  std::unordered_map<int, std::string> getOptions();
};

#endif
