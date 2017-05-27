#include "OptionParser.h"

// Print options and usage
void OptionParser::printHelp() {
  switch (cmdtype) {
  case TRAIN:
    std::cout << "  Usage: annet train <training_file> [options]\n\n"
      "  Options:\n"
      "  -i, --input-nodes\t\t Set number of input nodes\n"
      "  -h, --hidden-nodes\t\t Set number of hidden nodes\n"
      "  -o, --output-nodes\t\t Set number of output nodes\n"
      "  -l, --hidden-layers\t\t Set number of hidden layers\n"
      "  -w, --weight-file-name\t Specify basename of weight file\n"
      "  -d, --debug\t\t Turn on debug logging\n"
      "      --help\t\t\t Print this message\n\n"
      "  Example:\n"
      "  ./bin/annet train training_set.csv -h 5 -l 1\n\n";
    break;
  case TEST:
    std::cout << "  Usage: annet test <testing_file> <weight_file>\n\n"
      "  Options:\n"
      "  -p, --print-results\t Print the test results.\n"
      "  -d, --debug\t\t Turn on debug logging\n"
      "      --help\t\t Print this message\n\n"
      "  Example:\n"
      "  ./bin/annet test testing_set.csv weights.csv\n\n";
    break;
  }
}

// Print errors and return exit code 1
void OptionParser::error(int errorCode, std::string info) {
  switch (errorCode) {
  case ERRFILE :
    std::cerr << "  ERROR: File " << info << " is unreadable"
                 " or does not exist.\n";
    break;
  case ERRDIR :
    std::cerr << "  ERROR: Train class does not currently support"
                 " directories for training file argument.\n";
    break;
  case ERRNODE :
    std::cerr << "  ERROR: Invalid value for " << info << " nodes.\n";
    break;
  case ERRLAYER :
    std::cerr << "  ERROR: Invalid value for " << info << " layers.\n";
    break;
  case ERRNOTINT :
    std::cerr << "  ERROR: Expected argument of type int: " << info << std::endl;
    break;
  case ERROPT :
    std::cerr << "  ERROR: Invalid option \"" << info << "\".\n\n";
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
  if (std::stoi(options[HNODES]) < 0) {
    error(ERRNODE, "hidden");
  }
  if (std::stoi(options[HLAYERS]) < 0) {
    error(ERRLAYER, "hidden");
  }
  if (std::stoi(options[HNODES]) > 0 && std::stoi(options[HLAYERS]) <= 0) {
    error(ERRNOHNODES);
  }
  if (std::stoi(options[HNODES]) <= 0 && std::stoi(options[HLAYERS]) > 0) {
    error(ERRNOHLAYERS);
  }
}

// Parse test options
std::unordered_map<int, std::string> OptionParser::getTestOptions() {
  std::unordered_map<int, std::string> options;
  for (int i = 0; i < arguments.size(); i++) {
    std::string arg = arguments[i];
    if (arg == "--help") {
      printHelp();
      exit(0);
    }
    else if (arg == "-p" || arg == "--print-results") {
      options[PRNTRES] = "true";
    }
    else if (arg == "-d" || arg == "--debug") {
      options[DEBUG] = "true";
    }
    else if (options.count(TFILE) == 0) {
      options[TFILE] = testFile(arg);
    }
    else if (options.count(WFILE) == 0) {
      options[WFILE] = testFile(arg);
    }
    else {
      error(ERROPT, arg);
    }
  }

  if (options.count(TFILE) == 0) {
    error(ERRNOFILE, "Testing");
  } 
  if (options.count(WFILE) == 0) {
    error(ERRNOFILE, "Weight");
  }
  return options;
}

// Parse train options
std::unordered_map<int, std::string> OptionParser::getTrainOptions() {

  std::unordered_map<int, std::string> options;
  options[INODES] = "0";
  options[HNODES] = "0";
  options[ONODES] = "0";
  options[HLAYERS] = "0"; 
  options[WFILENAME] = "weights";

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
    else if (arg == "-w" || arg == "--weight-file-name") {
      options[WFILENAME] = arguments[i+1];
      i++;
    }
    else if (arg == "-d" || arg == "--debug") {
      options[DEBUG] = "true";
    }
    else if (options.count(TFILE) == 0) {
      options[TFILE] = testFile(arg);
    }
    else {
      error(ERROPT, arg);
    }
  }

  if (options.count(TFILE) == 0) {
   error(ERRNOFILE, "Training");
  } 
  testNodes(options);
  return options;
}

// Return options
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

// Public constructor
OptionParser::OptionParser(std::vector<std::string> args, int type) {
  arguments = args;
  cmdtype = type;
}
