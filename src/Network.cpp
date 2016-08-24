#include "Network.h"
  
Network::Network(int in, int hid, int out, int lay)
{
  numInputNodes = in;
  numHiddenNodes = hid;
  numOutputNodes = out;
  numHiddenLayers = lay;
  initNetwork();
}

// Initialize Network Layers
void Network::initNetwork()
{
  // Initialize input layer
  Layer layer = Layer(getNumInputNodes(), 0);
  layers.push_back(layer);

  // Initialize hidden layers
  for (int hid = 0; hid < getNumHiddenLayers(); hid++) {
    if (hid == 0) { 
      layer = Layer(getNumHiddenNodes(), getNumInputNodes());
    }
    else {
     layer = Layer(getNumHiddenNodes(), getNumHiddenNodes());
    }
    layers.push_back(layer);
  }

  // Initialize output leyer
  if (getNumHiddenNodes() == 0) {
    layer = Layer(getNumOutputNodes(), getNumInputNodes());
  }
  else {
    layer = Layer(getNumOutputNodes(), getNumHiddenNodes());
  }
  layers.push_back(layer);
}

float Network::calculateTotalError(std::vector<float> actualOutputs, std::vector<float> expectedOutputs) {
  float totalError = 0.0;
  return totalError;
}

void Network::train(std::vector<float> inputs, std::vector<float> expectedOutputs) {
  int layer  = 0;
  for (std::vector<Layer>::iterator it = layers.begin() ; it != layers.end(); ++it) {
    std::cout << "Layer " << layer << '\n';
    inputs = it->feedForward(inputs);
    layer++;
  }
  std::vector<float> actualOutputs = inputs;
  
  // Calulate total error?

  for (std::vector<Layer>::iterator it = layers.end() ; it != layers.begin(); --it) {
    std::cout << "Layer " << layer << '\n';
    deltas = it->backPropegation(deltas);
    layers--;
  }
}
