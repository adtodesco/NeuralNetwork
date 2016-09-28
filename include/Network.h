#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <fstream>

#include "Layer.h"

class Network
{
private:
  int numInputNodes, numOutputNodes, numHiddenNodes, numHiddenLayers;
  std::vector<Layer> layers;

  int parseHeader(std::string line);
  std::vector<float> parseWeights(std::string line);

  float calculateTotalError(std::vector<float> actualOutputs, std::vector<float> targetOutputs);
  std::vector<float> calculateInitialDeltinis(std::vector<float> actualOutputs, std::vector<float> targetOutputs);
  bool fileExists(std::string filename);
  std::string intToString(int i);
public:
  Network(int in, int hid, int out, int lay);
  Network(std::string weightFile);
  void initNetwork(std::vector< std::vector<float> > weights);

  float train(std::vector<float> inputs, std::vector<float> targetOutputs);
  void writeWeightFile(std::string weightsDir, std::string baseName = "weights");
  
  int getNumInputNodes() { return numInputNodes; }
  int getNumOutputNodes() { return numOutputNodes; }
  int getNumHiddenNodes() { return numHiddenNodes; }
  int getNumHiddenLayers() { return numHiddenLayers; }
};

#endif
