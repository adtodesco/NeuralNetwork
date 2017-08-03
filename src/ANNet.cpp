#include <iostream>
#include <vector>
#include <sys/stat.h>

#include "OptionParser.h"
#include "Network.h"
#include "Constants.h"

// Debug printing - TODO: Replace with logging
void debug(std::string debug, std::string log) {
  if (debug == "true") { std::cout << "DEBUG: " << log << std::endl; }
}

// Print usage
void printUsage() {
    std::cout << "  Usage: annet {train|test} <training_file> <weight_file> [options]\n\n";
}

// Basic argument checks
void checkArgs(int argc, char* argv[]) {

  // Check for help flag
  if (argc > 1 && strstr(argv[1], "help")) {
    printUsage();
    exit(0);
  }

  // Test for proper annet command
  if (argc < 2 || (strcmp(argv[1], "train") != 0 && strcmp(argv[1], "test") != 0 )) {
    std::cerr << "  ERROR: Unrecognized command\n";
    printUsage();
    exit(1);
  }

}

// Init network
Network initNetwork(std::unordered_map<int, std::string> options) {
  Network myNeuralNet = Network();
  std::ifstream infile(options[WFILE]);
  if (infile.good()) {
    //debug(options[DEBUG], "Initializing Neural Network with parameters weight file = "
    //        + options[WFILE]);

    myNeuralNet = Network(options[WFILE]);
  }
  else {
   //debug(options[DEBUG], "Initializing Neural Network with input nodes = "
   //         + options[INODES] + ", output nodes = " 
   //         + options[ONODES] + ", hidden nodes = " 
   //        + options[HNODES] + ", hidden layers = "
   //         + options[HLAYERS]);

    myNeuralNet = Network(std::stoi(options[INODES]),
                          std::stoi(options[HNODES]),
                          std::stoi(options[ONODES]),
                          std::stoi(options[HLAYERS]));
  }
  
  //debug(options[DEBUG], "Neural Network initialized.");

  return myNeuralNet;
}

std::vector< std::vector<float> > readTFile(Network myNeuralNet, std::string tFileName) {

  //debug(options[DEBUG], "Reading " + options[TFILE] + "...");

  // Variables
  std::ifstream tFile(tFileName);
  std::string val;
  std::vector< std::vector<float> > tData;

  // Iterate through tFile and read into tData
  while (std::getline(tFile, val)) {

    //debug(options[DEBUG], "Reading new line \"" + val + "\".");
    
    std::vector<float> temp;
    
    while (val.size() > 0) {
      size_t index = val.find(',');
      temp.push_back(std::stof(val.substr(0, index)));
      val = val.substr(index + 1, val.size() - 1);
    }

    tData.push_back(temp);
  }

  //debug(options[DEBUG], "Finished reading " + options[TFILE] + '.');

  return tData;
}

// Main method
int main(int argc, char* argv[]) {
  
  // Basica argument checks
  checkArgs(argc, argv);

  // Parse arguments
  std::vector<std::string> arguments(argv + 2, argv + argc); 
  int cmdType = (strcmp(argv[1], "train") == 0) ? TRAIN : TEST;

  OptionParser parser = OptionParser(arguments, cmdType);
  std::unordered_map<int, std::string> options = parser.getOptions();

  debug(options[DEBUG], "Debug logging is on.");

  // Initialize network
  Network myNeuralNet = initNetwork(options);
 
  // Read tFile into tData vector 
  std::vector< std::vector<float> > tData = readTFile(myNeuralNet, options[TFILE]);

  // Train
  if (cmdType == TRAIN) {
    
    debug(options[DEBUG], "Training Neural Network...");

    float totalError = 1.0;

    std::cout << "Epoch options: " << std::stoi(options[EPOCHS]) << std::endl;      
    for (int epoch = 0; epoch < std::stoi(options[EPOCHS]); epoch++) { 
      std::cout << "Epoch: " << epoch << std::endl;      
      for (std::vector< std::vector<float> >::iterator line = tData.begin(); line != tData.end(); ++line) {

        int output = line->back();
        line->pop_back();
        std::vector<float> inputVec = *line; 
        std::vector<float> outputVec(myNeuralNet.getNumOutputNodes(), 0.01);
        outputVec[output] = 0.99;
        
        totalError = myNeuralNet.train(inputVec, outputVec);
      }
     
    }

    std::string weightFileName = (options.find(WFILENAME) == options.end()) ? options[WFILE] : options[WFILENAME];
    debug(options[DEBUG], "Writing weight file " + weightFileName + '.');  
    myNeuralNet.writeWeightFile(weightFileName);
  }
  // Test
  else {
    
    debug(options[DEBUG], "Testing Neural Network...");

    int testCorrect = 0, testTotal = 0;

    for (std::vector< std::vector<float> >::iterator line = tData.begin(); line != tData.end(); ++line) {
      int result = 0;
      int output = line->back();
      line->pop_back();
      std::vector<float> inputVec = *line; 
      
      if (options[PRNTRES] == "true") {
        result = myNeuralNet.test(inputVec, output);
      }
      else {
        result = myNeuralNet.test(inputVec, output);
      }
      if (result == 1) {
        testCorrect++;
      }
      
      testTotal++;
    }

    std::cout << "Final test results:\n";
    std::cout << "  Correctly predicted " << testCorrect << " out of " << testTotal << " tests.\n";
  }

  return 0;
}

/*
    // Check tFile matches up with parameters
    if (output < 0 || output > myNeuralNet.getNumOutputNodes() - 1) {
      std::cerr << " ERROR: Output value \"" << output << "\" is out of range."
        " Expected " << myNeuralNet.getNumOutputNodes() << " possible outputs.\n";
      std::cerr << "        Outputs must be consecutive non-negative integers\n";
      exit(1);
    }

    debug(options[DEBUG], "Creating input vector...");
    debug(options[DEBUG], "Creating output vector...");


    // Check training file matches up with parameters
    if (inputCheck != myNeuralNet.getNumInputNodes()) {
      std::cerr << " ERROR: Expected " << myNeuralNet.getNumInputNodes() << " inputs per line."
        " Training/testing file has " << inputCheck << " inputs per line.\n";
      exit(1);
    }
*/
