#include "OptionParser.h"

// Public constructor
OptionParser::OptionParser(std::vector< std::string > args, int type) {
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
    case TEST:
      std::cout << " TEST COMMAND OPTION\n";
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
      std::cerr << " ERROR: Train class does not currently support "
        "directories for training file argument\n";
      break;
    case ERRNODE :
      std::cerr << "  ERROR: Must have at least one " << info << " node.\n";
      break;
    default: 
      std::cerr << " ERROR: Unknown error.\n";
      break;
  }
  exit(1);
}

// Validate option is an integer
bool OptionParser::isValInt(std::string opt, std::string val) {
  try {
    std::stoi(val);
  }
  catch (std::invalid_argument ia) {
    std::cerr << "  ERROR: Invalid value '" << val << "' for option '"
      << opt << "' (Value is not an Int)\n";
  }
  return true;
}

// Check parameters
void OptionParser::checkParams(std::vector< std::string > options) {
  if (std::stoi(options[0]) <= 0) {
    error(ERRNODE, "input");
  }
  if (std::stoi(options[2]) <= 0) {
    error(ERRNODE, "output");
  }
  if (std::stoi(options[1]) > 0 && std::stoi(options[3]) <= 0) {
    std::cerr << "  ERROR: Hidden layers are set to one or greater"
      " but hidden nodes are set to zero.\n";
    exit(1);
  }
  if (std::stoi(options[1]) <= 0 && std::stoi(options[3]) > 0) {
    std::cerr << "  ERROR: Hidden nodes are set to one or greater"
      " but hidden layers are set to zero.\n";
    exit(1);
  }
  // TODO: Confirm number of inputs matches (first line - 1) of .tra file
  // Get number of output nodes if unspecified
}

std::vector< std::string > OptionParser::getTestOptions() {
  std::vector<std::string> options;
  return options;
}

// Parse options
std::vector<std::string> OptionParser::getTrainOptions() {

  std::string trainingFile = "NULL";
  std::string inputNodes = "0";
  std::string hiddenNodes = "0";
  std::string outputNodes = "0";
  std::string hiddenLayers = "0"; 

  std::vector<std::string> options;
  options.push_back(inputNodes);
  options.push_back(hiddenNodes);
  options.push_back(outputNodes);
  options.push_back(hiddenLayers);
  options.push_back(trainingFile);
  
  for (int i = 0; i < arguments.size(); i++) {
    std::string arg = arguments[i];
    if (arg == "--help") {
      printHelp();
      exit(0);
    }
    else if (arg == "-i" || arg == "--input-nodes") {
      isValInt(arguments[i], arguments[i+1]);
      options[0] = arguments[i+1];
      i++;
    }
    else if (arg == "-h" || arg == "--hidden-nodes") {
      isValInt(arguments[i], arguments[i+1]);
      options[1] = arguments[i+1];
      i++;
    }
    else if (arg == "-o" || arg == "--output-nodes") {
      isValInt(arguments[i], arguments[i+1]);
      options[2] = arguments[i+1];
      i++;
    }
    else if (arg == "-l" || arg == "--hidden-layers") {
      isValInt(arguments[i], arguments[i+1]);
      options[3] = arguments[i+1];
      i++;
    }
    else if (trainingFile == "NULL") {
      trainingFile = arguments[i];
      std::ifstream tFile(trainingFile);
      if (tFile.good()) {
	options[4] = arguments[i];
      }
      else {
	error(ERRFILE, trainingFile);
      }
    }
    else {
      std::cerr << "  ERROR: Invalid option \"" << arguments[i] << "\"\n\n";
      printHelp();
      exit(1);
    }
  }

  if (trainingFile == "NULL") {
   std::cerr << "  ERROR: Training file was not provided.\n\n";
   printHelp();
   exit(1);
  } 

  checkParams(options); 
  return options;
}

std::vector<std::string> OptionParser::getOptions() {
  std::vector<std::string> options;
  switch (cmdtype) {
  case TRAIN:
    options = getTrainOptions();
  case TEST:
    options = getTestOptions();
  }
  return options;
}
