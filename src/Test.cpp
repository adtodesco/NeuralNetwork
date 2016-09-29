#include "Network.h"

int main(int argc, char* argv[]) {
  Network myNet = Network(argv[1]);
  
  std::ifstream tFile(argv[2]);
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

}
