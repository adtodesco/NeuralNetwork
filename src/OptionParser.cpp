#include "OptionParser.h"

// Public constructor
OptionParser::OptionParser(std::vector<std::string> args, int type) {
  arguments = args;
  cmdtype = type;
}

// Print options and usage
void OptionParser::printHelp() {
  switch (cmdtype) {
  case TRAIN:
    std::cout << "  Usage: trainer <training_file> [options]\n\n"
      "  Options:\n"
      "  -i, --input-nodes\t Set number of input nodes\n"
      "  -h, --hidden-nodes\t Set number of hidden nodes\n"
      "  -o, --output-nodes\t Set number of output nodes\n"
      "  -l, --hidden-layers\t Set number of hidden layers\n"
      "      --help\t\t Print this message\n\n"
      "  Example:\n"
      "  ./bin/trainer training_set.tra -h 5 -l 1\n\n";
    break;
  case TEST:
    std::cout << " TEST COMMAND OPTION\n";
    break;
  }
}

// Print errors and return exit code 1
void OptionParser::error(int errorCode, std::string info) {
  switch (errorCode) {
  case ERRFILE :
    std::cerr << "  ERROR: Training file " << info << " is "
                 "unreadable or does not exist\n";
    break;
  case ERRDIR :
    std::cerr << "  ERROR: Train class does not currently support "
                 "directories for training file argument\n";
    break;
  case ERRNODE :
    std::cerr << "  ERROR: Must have at least one " << info << " node.\n";
    break;
  case ERRNOTINT :
    std::cerr << "  ERROR: Expected argument of type int: " << info << std::endl;
    break;
  case ERROPT :
    std::cerr << "  ERROR: Invalid option \"" << info << "\"\n\n";
    printHelp();
    break;
  case ERRNOFILE :
    std::cerr << "  ERROR: " << info << " file was not provided.\n\n";
    printHelp();
    break;
  case ERRNOHLAYERS :
    std::cerr << "  ERROR: Hidden nodes are set to one or greater"
                 " but hidden layers are set to zero.\n";
    break;
  case ERRNOHNODES :
    std::cerr << "  ERROR: Hidden layers are set to one or greater"
                 " but hidden nodes are set to zero.\n";
    break;
  default: 
    std::cerr << "  ERROR: Unknown error.\n";
  }
  exit(1);
}

// Validate option is a valid file
std::string OptionParser::testFile(std::string val) {
  std::ifstream tFile(val);
  if (!tFile.good()) {
    error(ERRFILE, val);
  }
  return val;
}

// Validate option is an integer
std::string OptionParser::testInt(std::string val) {
  try {
    std::stoi(val);
  }
  catch (std::invalid_argument ia) {
    error(ERRNOTINT, val);
  }
  return val;
}

// Test node/layer configuration
void OptionParser::testNodes(std::unordered_map<int, std::string> options) {
  if (std::stoi(options[INODES]) <= 0) {
    error(ERRNODE, "input");
  }
  if (std::stoi(options[ONODES]) <= 0) {
    error(ERRNODE, "output");
  }
  if (std::stoi(options[HNODES]) > 0 && std::stoi(options[HLAYERS]) <= 0) {
    error(ERRNOHNODES);
  }
  if (std::stoi(options[HNODES]) <= 0 && std::stoi(options[HLAYERS]) > 0) {
    error(ERRNOHLAYERS);
  }
}

std::unordered_map<int, std::string> OptionParser::getTestOptions() {
  std::unordered_map<int, std::string> options;
  return options;
}

// Parse options
std::unordered_map<int, std::string> OptionParser::getTrainOptions() {

  std::unordered_map<int, std::string> options;
  options[INODES] = "0";
  options[HNODES] = "0";
  options[ONODES] = "0";
  options[HLAYERS] = "0"; 

  for (int i = 0; i < arguments.size(); i++) {
    std::string arg = arguments[i];
    if (arg == "--help") {
      printHelp();
      exit(0);
    }
    else if (arg == "-i" || arg == "--input-nodes") {
      options[INODES] = testInt(arguments[i+1]);
      i++;
    }
    else if (arg == "-h" || arg == "--hidden-nodes") {
      options[HNODES] = testInt(arguments[i+1]);
      i++;
    }
    else if (arg == "-o" || arg == "--output-nodes") {
      options[ONODES] = testInt(arguments[i+1]);
      i++;
    }
    else if (arg == "-l" || arg == "--hidden-layers") {
      options[HLAYERS] = testInt(arguments[i+1]);
      i++;
    }
    else if (options.count(TRFILE) == 0) {
      options[TRFILE] = testFile(arguments[i]);
    }
    else {
      error(ERROPT, arg);
    }
  }

  if (options.count(TRFILE) == 0) {
   error(ERRNOFILE, "Training");
  } 
  testNodes(options);
  return options;
}

std::unordered_map<int, std::string> OptionParser::getOptions() {
  std::unordered_map<int, std::string> options;
  switch (cmdtype) {
  case TRAIN:
    options = getTrainOptions();
    break;
  case TEST:
    options = getTestOptions();
    break;
  }
  return options;
}
