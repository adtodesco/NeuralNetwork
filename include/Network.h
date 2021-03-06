#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <fstream>

#include "Layer.h"
#include "WeightFile.h"

class Network
{
private:
  float calculateTotalError(std::vector<float> actualOutputs, std::vector<float> targetOutputs);
  std::vector<float> calculateInitialDeltinis(std::vector<float> actualOutputs, std::vector<float> targetOutputs);
  int testResults(std::vector<float> outputs, int actualOutput);
  int testAndPrintResults(std::vector<float> outputs, int actualOutput);
  
  void initNetwork(std::vector< std::vector<float> > weights);

  int numInputNodes, numOutputNodes, numHiddenNodes, numHiddenLayers;
  std::vector<Layer> layers;
public:
  Network(int in, int hid, int out, int lay);
  Network(std::string weightFile);
  Network() {};

  float train(std::vector<float> inputs, std::vector<float> targetOutputs);
  int test(std::vector<float> inputs, int output, bool print = false);
  std::vector<float> predict(std::vector<float> inputs);

  void writeWeightFile(std::string weightFile = "weights");
  
  int getNumInputNodes() { return numInputNodes; }
  int getNumOutputNodes() { return numOutputNodes; }
  int getNumHiddenNodes() { return numHiddenNodes; }
  int getNumHiddenLayers() { return numHiddenLayers; }
};

#endif
