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

// Parse options
std::vector<std::string> parseOptions(int argc, char* argv[]) {

  if (argc < 2) {
   std::cerr << "  ERROR: Training file was not provided.\n\n";
   printHelp();
   exit(1);
  } 

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
    //std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
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
      std::ifstream tFile(trainingFile);
      if (tFile.good()) {
        options[4] = argv[i];
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

  // Check training file to confirm inputs, get outputs if not specified
  // Warn user that getting outputs may take a while
  // std::vector<int> getNodes(training file)
  Network myNeuralNet = Network(std::stoi(options[0]),
                                std::stoi(options[1]),
                                std::stoi(options[2]),
                                std::stoi(options[3]));

  // Variables
  int numInputs = std::stoi(options[0]);
  int numOutputs = std::stoi(options[2]);
  std::ifstream tFile(options[4]);
  std::string val;

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
    // TODO: Error checking - make sure output vector index is int and in bounds of numOutputs
    
    std::cout << "Input vector:\n";
    for (std::vector<float>::iterator it = inputVec.begin() ; it != inputVec.end(); ++it) {
      std::cout << *it << std::endl;
    }
   
    std::cout << "Output vector:\n";
    for (std::vector<float>::iterator it = outputVec.begin() ; it != outputVec.end(); ++it) {
      std::cout << *it << std::endl;
    }

    float totalError = 1.0;
    int iterations = 0;
    while (totalError > 0.001) {
      totalError = myNeuralNet.train(inputVec, outputVec);
      if (iterations % 100 == 0) {
        std::cout << totalError << std::endl;
      }
      iterations++;
    }
    std::cout << "  ===============\n";
    std::cout << "  Iterations: " << iterations << std::endl;
    std::cout << "  Total Error: " << totalError << std::endl;
    std::cout << "  ===============\n";
 }

  myNeuralNet.writeWeightFile(getWeightsDir(argv[0]));
  return 0;
}
