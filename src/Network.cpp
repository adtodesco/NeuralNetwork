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
  int layer  = 0;
  for (std::vector<Layer>::iterator it = layers.begin() ; it != layers.end(); ++it) {
    std::cout << "Layer " << layer << '\n';
    inputs = it->feedForward(inputs);
    layer++;
  }
}
