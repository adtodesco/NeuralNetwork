#include <iostream>
#include <vector>
#include <sys/stat.h>

#include "Network.h"

// Returns the path to the default weights directory
std::string getWeightsDir(char* argvZero) {
  std::string executablePath = realpath(argvZero, NULL);
  size_t index = executablePath.rfind('/', executablePath.rfind('/') - 1);
  return executablePath.substr(0, index) + "/files/weights";
}

// Print options and usage
void printHelp() {
  std::cout << "  Usage: trainer <training_file> [options]\n\n"
    "  Options:\n"
    "  -i, --input-nodes\t Set number of input nodes\n"
    "  -h, --hidden-nodes\t Set number of hidden nodes\n"
    "  -o, --output-nodes\t Set number of output nodes\n"
    "  -l, --hidden-layers\t Set number of hidden layers\n"
    "      --help\t\t Print this message\n\n"
    "  Example:\n"
    "  ./bin/trainer training_set.tra -h 5 -l 1\n\n";
}

// Validate option is an integer
void isValInt(char* opt, char* val) {
  std::string valStr = val;
  try {
    std::stoi(valStr);
  }
  catch (std::invalid_argument ia) {
    std::cerr << "  ERROR: Invalid value '" << val << "' for option '"
      << opt << "' (Value is not an Int)\n";
   }
}

// Print errors and return exit code 1
void error(int errorCode, std::string info = "") {
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

// Check parameters
void checkParams(std::vector< std::string > options) {
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

// Parse options
std::vector<std::string> parseOptions(int argc, char* argv[]) {

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
  
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "--help") {
      printHelp();
      exit(0);
    }
    else if (arg == "-i" || arg == "--input-nodes") {
      isValInt(argv[i], argv[i+1]);
      options[0] = argv[i+1];
      i++;
    }
    else if (arg == "-h" || arg == "--hidden-nodes") {
      isValInt(argv[i], argv[i+1]);
      options[1] = argv[i+1];
      i++;
    }
    else if (arg == "-o" || arg == "--output-nodes") {
      isValInt(argv[i], argv[i+1]);
      options[2] = argv[i+1];
      i++;
    }
    else if (arg == "-l" || arg == "--hidden-layers") {
      isValInt(argv[i], argv[i+1]);
      options[3] = argv[i+1];
      i++;
    }
    else if (trainingFile == "NULL") {
      trainingFile = argv[i];
      struct stat s;
      if (stat(argv[i], &s) == 0) {
        if (s.st_mode & S_IFREG) {
          std::ifstream tFile(trainingFile);
          if (tFile.good()) {
            options[4] = argv[i];
          }
          else {
            error(ERRFILE, trainingFile);
          }
        }
        else if (s.st_mode & S_IFDIR) {
          // TODO: Support directories as training file argument
          error(ERRDIR);
        }
        else {
          error(ERRFILE, trainingFile);
        }
      }
      else {
        error(ERRFILE, trainingFile);
      }
    }
    else {
      std::cerr << "  ERROR: Invalid option \"" << argv[i] << "\"\n\n";
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

// Main method
int main(int argc, char* argv[]) {

  std::vector< std::string > options = parseOptions(argc, argv);

  // Initialize network
  Network myNeuralNet = Network(std::stoi(options[0]),
                                std::stoi(options[1]),
                                std::stoi(options[2]),
                                std::stoi(options[3]));

  // Variables
  int numInputs = std::stoi(options[0]);
  int numOutputs = std::stoi(options[2]);
  std::ifstream tFile(options[4]);
  std::string val;

  float totalError = 1.0;
/*
  // Iterate through training file
  while (!tFile.eof()) {
    std::vector<float> inputVec;
    std::vector<float> outputVec(numOutputs, 0.01);
    // Set input and output vectors
    for (int i = 0; i < numInputs; i++) {
      std::getline(tFile, val, ',');
      inputVec.push_back(std::stof(val));
    }
    std::getline(tFile, val, ',');
    outputVec[std::stoi(val)] = 0.99;
   
    totalError = myNeuralNet.train(inputVec, outputVec);
  }
  myNeuralNet.writeWeightFile(getWeightsDir(argv[0]));
*/  return 0;
}
