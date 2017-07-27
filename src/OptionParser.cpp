#include "OptionParser.h"

// Print options and usage
void OptionParser::printHelp() {
  switch (cmdtype) {
  case TRAIN:
    std::cout << "  Usage: annet train <training_file> <weight_file> [options]\n\n"
      "  Options:\n"
      "  -i, --input-nodes\t\tSet number of input nodes\n"
      "  -h, --hidden-nodes\t\tSet number of hidden nodes\n"
      "  -o, --output-nodes\t\tSet number of output nodes\n"
      "  -l, --hidden-layers\t\tSet number of hidden layers\n"
      "  -w, --weight-file-name\tSet name of weight file (maintains original weight file)\n"
      "  -d, --debug\t\t\tTurn on debug logging\n"
      "      --help\t\t\tPrint this message\n\n"
      "  Examples:\n"
      "  annet train training_set.csv weights.csv -i 5 -o 2 -h 5 -l 1 (new network)\n"
      "  annet train training_set.csv weights.csv (existing network)\n\n"
      "  Note that the node and layer arguments are only required when training a new\n"
      "  network (no weight file). These options will be ignored in favor of the weight\n"
      "  file metadata.\n\n";
    break;
  case TEST:
    std::cout << "  Usage: annet test <testing_file> <weight_file> [options]\n\n"
      "  Options:\n"
      "  -p, --print-results\tPrint the test results.\n"
      "  -d, --debug\t\tTurn on debug logging\n"
      "      --help\t\tPrint this message\n\n"
      "  Example:\n"
      "  annet test testing_set.csv weights.csv\n\n";
    break;
  }
}

// Print errors and return exit code 1
void OptionParser::error(int errorCode, std::string info) {
  switch (errorCode) {
  case ERRFILE :
    std::cerr << "  ERROR: File " << info << " already exists.\n";
    break;
  case ERRNOFILE :
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
  case ERRNOARG :
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
bool OptionParser::fileExist(std::string val) {
  std::ifstream tFile(val);
  return tFile.good();
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
  
  // Confirm miminum arguments of training and weight files
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
      if (!fileExist(arg)) {
          error(ERRNOFILE, arg);
      }
      options[TFILE] = arg; 
    }
    else if (options.count(WFILE) == 0) {
      if (!fileExist(arg)) {
          error(ERRNOFILE, arg);
      }
      options[WFILE] = arg;
    }
    else {
      error(ERROPT, arg);
    }
  }

  // Confirm miminum arguments of testing and weight files
  if (options.count(TFILE) == 0) {
    error(ERRNOARG, "Testing");
  } 
  if (options.count(WFILE) == 0) {
    error(ERRNOARG, "Weight");
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

  // Check arguments
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
      if (fileExist(arguments[i+1])) {
        error(ERRFILE, arg);
      }
      options[WFILENAME] = arguments[i+1];
      i++;
    }
    else if (arg == "-d" || arg == "--debug") {
      options[DEBUG] = "true";
    }
    else if (options.count(TFILE) == 0) {
      if (!fileExist(arg)) {
          error(ERRNOFILE, arg);
      }
      options[TFILE] = arg; 
    }
    else if (options.count(WFILE) == 0) {
      options[WFILE] = arg; 
    }
    else {
      error(ERROPT, arg);
    }
  }

  // Confirm miminum arguments of training and weight files
  if (options.count(TFILE) == 0) {
    error(ERRNOARG, "Training");
  } 
  if (options.count(WFILE) == 0) {
    error(ERRNOARG, "Weight");
  }

  // Confirm node options are valid on intial training set 
  // (empty weight file)
  if (!fileExist(options[WFILE])) {
    testNodes(options);
  }

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
