#include "Network.h"
  
Network::Network(int in, int out, int hid, int lay)
{
  numInputNodes = in;
  numOutputNodes = out;
  numHiddenNodes = hid;
  numHiddenLayers = lay;
  initNetwork();
}

// Initialize Network Layers
void Network::initNetwork()
{
  // Initialize input layer
  Layer layer = Layer(numInputNodes, 0);
  layers.push_back(layer);

  // Initialize hidden layers
  for (int hid = 0; hid < numHiddenLayers; hid++) {
    if (hid == 0) { 
      layer = Layer(numHiddenNodes, numInputNodes);
    }
    else {
     layer = Layer(numHiddenNodes, numHiddenNodes);
    }
    layers.push_back(layer);
  }

  // Initialize output leyer
  if (numHiddenNodes == 0) {
    layer = Layer(numOutputNodes, numInputNodes);
  }
  else {
    layer = Layer(numOutputNodes, numHiddenNodes);
  }
  layers.push_back(layer);
}

void Network::train(std::vector<float> inputs) {
  for (std::vector<Layer>::iterator it = layers.begin() ; it != layers.end(); ++it) {
    inputs = it->feedForward(inputs);
  }
}
