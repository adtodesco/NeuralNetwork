#include <iostream>
#include <vector>
#include <sys/stat.h>

#include "OptionParser.h"
#include "Network.h"
#include "Constants.h"

// Print usage
void printUsage() {

}

// Returns the path to the default weights directory
std::string getWeightsDir(char* argvZero) {
  std::string executablePath = realpath(argvZero, NULL);
  size_t index = executablePath.rfind('/', executablePath.rfind('/') - 1);
  return executablePath.substr(0, index) + "/files/weights";
}

// Main method
int main(int argc, char* argv[]) {

  // Test for proper annet command
  if (argc < 2 || (strcmp(argv[1], "train") != 0 && strcmp(argv[1], "test") != 0 )) {
    std::cerr << "Command not recognized.\n";
    //printUsage();
    exit(1);
  }

  std::vector<std::string> arguments(argv + 2, argv + argc); 
  int cmdType = (strcmp(argv[1], "train") == 0) ? TRAIN : TEST;

  OptionParser parser = OptionParser(arguments, cmdType);
  std::unordered_map<int, std::string> options = parser.getOptions();

/*

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
  // Iterate through training file
  while (std::getline(tFile, val)) {
    std::vector<float> inputVec;
    std::vector<float> outputVec(numOutputs, 0.01);
    size_t index = val.rfind(',');
    int output = std::stoi(val.substr(index + 1, val.size() -1));

    // Check training file matches up with parameters
    if (output < 0 || output > numOutputs - 1) {
      std::cerr << " ERROR: Output value \"" << output << "\" is out of range."
        " Expected " << numOutputs << " possible outputs.\n";
      std::cerr << " (NOTE: Outputs must be consecutive non-negative integers)\n";
      exit(1);
    }
    outputVec[output] = 0.99;

    int inputCheck = 0;
    // Set input and output vectors
    val = val.substr(0, index + 1);
    while (val.size() > 0) {
      index = val.find(',');
      inputVec.push_back(std::stof(val.substr(0, index)));
      val = val.substr(index + 1, val.size() - 1);
      inputCheck++;
    }

    // Check training file matches up with parameters
    if (inputCheck != numInputs) {
      std::cerr << " ERROR: Expected " << numInputs << " inputs per line."
        " Training file has " << inputCheck << " inputs per line.\n";
      exit(1);
    }

    totalError = myNeuralNet.train(inputVec, outputVec);
  }
  myNeuralNet.writeWeightFile(getWeightsDir(argv[0]));
*/
  return 0;
}
