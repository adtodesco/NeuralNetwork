#include <iostream>
#include <vector>
#include <sys/stat.h>

#include "OptionParser.h"
#include "Network.h"
#include "Constants.h"

// Print usage
void printUsage() {

}

void debug(std::string debug, std::string log) {
  if (debug == "true") { std::cout << "DEBUG: " << log << std::endl; }
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

  debug(options[DEBUG], "Debug logging is on.");

  // Initialize network
  Network myNeuralNet = Network();
  std::ifstream infile(options[WFILE]);
  if (infile.good()) {
    debug(options[DEBUG], "Initializing Neural Network with parameters weight file = "
            + options[WFILE]);

    myNeuralNet = Network(options[WFILE]);
  }
  else {
   debug(options[DEBUG], "Initializing Neural Network with input nodes = "
            + options[INODES] + ", output nodes = " 
            + options[ONODES] + ", hidden nodes = " 
            + options[HNODES] + ", hidden layers = "
            + options[HLAYERS]);

    myNeuralNet = Network(std::stoi(options[INODES]),
                          std::stoi(options[HNODES]),
                          std::stoi(options[ONODES]),
                          std::stoi(options[HLAYERS]));
  }
  
  debug(options[DEBUG], "Neural Network initialized.");

  // Variables
  std::ifstream tFile(options[TFILE]);
  std::string val;

  debug(options[DEBUG], "Reading " + options[TFILE] + "...");

  float totalError = 1.0;
  int testCorrect = 0, testTotal = 0, testTemp = 0;
  std::vector<float> outputVector;
  // Iterate through training file
  while (std::getline(tFile, val)) {

    debug(options[DEBUG], "Reading new line \"" + val + "\".");
    debug(options[DEBUG], "Creating output vector...");

    std::vector<float> inputVec;
    std::vector<float> outputVec(myNeuralNet.getNumOutputNodes(), 0.01);
    size_t index = val.rfind(',');
    int output = std::stoi(val.substr(index + 1, val.size() - 1));

    // Check tFile matches up with parameters
    if (output < 0 || output > myNeuralNet.getNumOutputNodes() - 1) {
      std::cerr << " ERROR: Output value \"" << output << "\" is out of range."
        " Expected " << myNeuralNet.getNumOutputNodes() << " possible outputs.\n";
      std::cerr << "        Outputs must be consecutive non-negative integers\n";
      exit(1);
    }
    outputVec[output] = 0.99;

    debug(options[DEBUG], "Creating input vector...");

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
    if (inputCheck != myNeuralNet.getNumInputNodes()) {
      std::cerr << " ERROR: Expected " << myNeuralNet.getNumInputNodes() << " inputs per line."
        " Training/testing file has " << inputCheck << " inputs per line.\n";
      exit(1);
    }

    if (cmdType == TRAIN) {
      
      debug(options[DEBUG], "Training Neural Network...");
      
      totalError = myNeuralNet.train(inputVec, outputVec);
    }
    else {
      
      debug(options[DEBUG], "Testing Neural Network...");
      
      outputVector = myNeuralNet.test(inputVec, output);
      if (options[PRNTRES] == "true") {
        testTemp = myNeuralNet.testAndPrintResults(outputVector, output);
      }
      else {
        testTemp = myNeuralNet.testResults(outputVector, output);
      }
      if (testTemp == 1) {
        testCorrect++;
      }
      testTotal++;
    }
  }

  debug(options[DEBUG], "Finished reading " + options[TFILE] + '.');

  if (cmdType == TRAIN) {
    std::string weightFileName = (options.find(WFILENAME) == options.end()) ? options[WFILE] : options[WFILENAME];
    debug(options[DEBUG], "Writing weight file " + weightFileName + '.');  
    myNeuralNet.writeWeightFile(weightFileName);
  }
  else {
    std::cout << "Final test results:\n";
    std::cout << "  Correctly predicted " << testCorrect << " out of " << testTotal << " tests.\n";
  }
  return 0;
}
