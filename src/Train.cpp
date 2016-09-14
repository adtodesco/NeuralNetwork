#include "Train.h"

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
    "  -h, --hidden-nodes\t Set number of hidden nodes\n"
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

// Parse options
std::vector<std::string> parseOptions(int argc, char* argv[]) {

  if (argc < 2) {
   std::cerr << "  ERROR: Training file was not provided.\n\n";
   printHelp();
   exit(1);
  } 

  std::string trainingFile = "NULL";
  std::string hiddenNodes = "0";
  std::string hiddenLayers = "0"; 

  std::vector<std::string> options;
  options.push_back(trainingFile);
  options.push_back(hiddenNodes);
  options.push_back(hiddenLayers);
  
  for (int i = 1; i < argc; i++) {
    //std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    std::string arg = argv[i];
    if (arg == "--help") {
      printHelp();
      exit(0);
    }
    else if (arg == "-h" || arg == "--hidden-nodes") {
      isValInt(argv[i], argv[i+1]);
      options[1] = argv[i+1];
      i++;
    }
    else if (arg == "-l" || arg == "--hidden-layers") {
      isValInt(argv[i], argv[i+1]);
      options[1] = argv[i+1];
      i++;
    }
    else if (trainingFile == "NULL") {
      trainingFile = argv[i];
      std::ifstream tFile(trainingFile);
      if (tFile.good()) {
        options[0] = argv[i];
      }
      else {
        std::cerr << "  ERROR: Training file " << trainingFile << " is "
          "unreadable or does not exist\n";
        exit(1);
      }
    }
    else {
      std::cerr << "  ERROR: Invalid option \"" << argv[i] << "\"\n\n";
      printHelp();
      exit(1);
    }
  }
  return options;
}

int main(int argc, char* argv[]) {
  
  std::vector< std::string > options = parseOptions(argc, argv);

  Network myNeuralNet = Network(2, 5, 2, 1);
  //Network myNeuralNet = Network(10, 5, 7, 1);
  //Network myNeuralNet = Network(5, 5, 2, 1);

  static const float inputArr[] = {4.5, -2.3 };
  //static const float inputArr[] = {1.0,2.5,3.5,4.2,4.3,1.5,2.5,1.9,-1.0,-2.6};
  //static const float inputArr[] = {1.0,2.5,3.5,4.5,1.5};
  static const float outputArr[] = {0.99, 0.01};
  std::vector<float> inputVec (inputArr, inputArr + sizeof(inputArr) / sizeof(inputArr[0]) );
  std::vector<float> outputVec (outputArr, outputArr + sizeof(outputArr) / sizeof(outputArr[0]) );

  float totalError = 1.0;
  int iterations = 0;
  while (totalError > 0.0001) {
    iterations++;
    totalError = myNeuralNet.train(inputVec, outputVec);
  }
  std::cout << "===============\n";
  std::cout << "Iterations: " << iterations << std::endl;
  std::cout << "Total Error: " << totalError << std::endl;
  std::cout << "===============\n";

  myNeuralNet.writeWeightFile(getWeightsDir(argv[0]));
  return 0;
}
