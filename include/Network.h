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

public:
  Network(int in, int hid, int out, int lay);
  void initNetwork();

  void train(std::vector<float> inputs);
  void writeWeights();
  
  void test();

};

#endif
