#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>

#include "Layer.h"

class Network
{
private:
  int numInputNodes, numOutputNodes, numHiddenNodes, numHiddenLayers;
  std::vector<Layer> layers;
  float calculateTotalError(std::vector<float> actualOutputs, std::vector<float> targetOutputs);
  std::vector<float> calculateInitialDeltinis(std::vector<float> actualOutputs, std::vector<float> targetOutputs);

public:
  Network(int in, int hid, int out, int lay);
  void initNetwork();

  void train(std::vector<float> inputs, std::vector<float> targetOutputs);
  void writeWeights();
  
  int getNumInputNodes() { return numInputNodes; }
  int getNumOutputNodes() { return numOutputNodes; }
  int getNumHiddenNodes() { return numHiddenNodes; }
  int getNumHiddenLayers() { return numHiddenLayers; }
};

#endif
