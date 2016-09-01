#include "Network.h"

// Public constructor  
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
  // Initialize random seed
  srand(time(NULL));

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

// Calculate total error of the Network outputs w.r.t. the target outputs
float Network::calculateTotalError(std::vector<float> actualOutputs, std::vector<float> targetOutputs) {
  float error;
  float totalError = 0.0;
  for (int n = 0; n < actualOutputs.size(); n++) {
    error = 0.5 * pow((targetOutputs[n] - actualOutputs[n]), 2);
    totalError = totalError + error;
    std::cout << "Output Node " << n << ":\n";
    std::cout << "  Target: " << targetOutputs[n] << " Actual: " << actualOutputs[n] << '\n';
    std::cout << "  Error: " << error << '\n';
  }
  std::cout << "Total Error: " << totalError << '\n';
  return totalError;
}

// Calculate the deltinits for the output nodes
std::vector<float> Network::calculateInitialDeltinis(std::vector<float> actualOutputs, std::vector<float> targetOutputs) {
  std::vector<float> deltinis;
  for (int n = 0; n < actualOutputs.size(); n++) {
    deltinis.push_back(-1 * (targetOutputs[n] - actualOutputs[n]));
  }
  return deltinis;
}

// Train Network on a set of inputs and target outputs
float Network::train(std::vector<float> inputs, std::vector<float> targetOutputs) {
  // Loop forward
  for (std::vector<Layer>::iterator it = layers.begin(); it != layers.end(); ++it) {
    inputs = it->feedForward(inputs);
  }

  // Final inputs are the actual outputs
  float totalError = calculateTotalError(inputs, targetOutputs);
  std::vector<float> deltinis = calculateInitialDeltinis(inputs, targetOutputs);

  // Loop back and stop before input layer
  for (std::vector<Layer>::reverse_iterator rt = layers.rbegin(); rt != layers.rend() - 1; ++rt) {
    deltinis = rt->backPropegation(deltinis);
  }
  return totalError;
}
