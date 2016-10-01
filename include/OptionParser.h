#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H

#include <iostream>
#include <vector> //Temporary?
#include <fstream> //Temporary?
#include <sys/stat.h>

#include "Constants.h"

class OptionParser
{
private:
  bool isValInt(std::string opt, std::string val);

  void error(int errorCode, std::string info = "");
  void checkParams(std::vector< std::string > options);

  std::vector< std::string > getTestOptions();
  std::vector< std::string > getTrainOptions();

  int cmdtype;
  std::vector< std::string > arguments;
public:
  OptionParser(std::vector< std::string > args, int type);

  std::vector< std::string > getOptions();
  void printHelp();
};

#endif
