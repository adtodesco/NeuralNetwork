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

float Network::calculateTotalError(std::vector<float> actualOutputs, std::vector<float> targetOutputs) {
  float error;
  float totalError = 0.0;
  for (int n = 0; n < actualOutputs.size(); n++) {
    error = 0.5 * pow((targetOutputs[n] - actualOutputs[n]), 2);
    totalError = totalError + error;
    std::cout << "Node " << n << ":\n";
    std::cout << "  Target: " << targetOutputs[n] << " Actual: " << actualOutputs[n] << '\n';
    std::cout << "  Error: " << error << '\n';
  }
  std::cout << "Total Error: " << totalError << '\n';
  return totalError;
}

std::vector<float> Network::calculateInitialDeltinis(std::vector<float> actualOutputs, std::vector<float> targetOutputs) {
  std::vector<float> deltas;
  for (int n = 0; n < actualOutputs.size(); n++) {
    deltas.push_back(-1 * (targetOutputs[n] - actualOutputs[n]));
  }
  return deltas;
}

void Network::train(std::vector<float> inputs, std::vector<float> targetOutputs) {
  int layer = 0;
  for (std::vector<Layer>::iterator it = layers.begin(); it != layers.end(); ++it) {
    //std::cout << "Layer " << layer << '\n';
    inputs = it->feedForward(inputs);
    layer++;
  }

  // Final inputs are the actual outputs
  calculateTotalError(inputs, targetOutputs);
  std::vector<float> deltinis = calculateInitialDeltinis(inputs, targetOutputs);

  // Loop back and stop before input layer
  layer--;
  for (std::vector<Layer>::reverse_iterator rt = layers.rbegin(); rt != layers.rend() - 1; ++rt) {
    //std::cout << "Layer " << layer << '\n';
    rt->printLinks();
    deltinis = rt->backPropegation(deltinis);
    rt->printLinks();
    layer--;
  }
}
