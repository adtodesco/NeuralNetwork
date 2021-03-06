#include "Network.h"

// Writes neural network weights to file
void Network::writeWeightFile(std::string weightFile) {

  std::vector< std::vector<float> > weights;
  int lay = 0;
  std::vector< std::vector<Link> > links;

  // Build weights
  for (std::vector<Layer>::iterator it = layers.begin() + 1; it != layers.end(); ++it) {
    weights.push_back(std::vector<float>());
    links = it->getLinks();
    for (std::vector< std::vector<Link> >::iterator node = links.begin(); node != links.end(); ++node) {
      for (std::vector<Link>::iterator prevNode = node->begin(); prevNode != node->end(); ++prevNode) {
        weights[lay].push_back(prevNode->getWeight());
      }
    }
    lay++;
  }

  WeightFile file = WeightFile(getNumInputNodes(),
                               getNumHiddenNodes(),
                               getNumOutputNodes(),
                               getNumHiddenLayers(),
                               weights);

  file.writeWeightFile(weightFile);
}

// Calculate total error of the Network outputs w.r.t. the target outputs
float Network::calculateTotalError(std::vector<float> actualOutputs, std::vector<float> targetOutputs) {
  float error;
  float totalError = 0.0;
  for (int n = 0; n < actualOutputs.size(); n++) {
    error = 0.5 * pow((targetOutputs[n] - actualOutputs[n]), 2);
    totalError = totalError + error;
    //std::cout << "Output Node " << n << ':' << std::endl;
    //std::cout << "  Target: " << targetOutputs[n] << " Actual: " << actualOutputs[n] << std::endl;
    //std::cout << "  Error: " << error << std::endl;
  }
  //std::cout << "Total Error: " << totalError << std::endl;
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

// Test prediction - returns 1 if correct, -1 if incorrect, 0 if no actualOutput given
int Network::testResults(std::vector<float> outputs, int actualOutput) {
  int node = 0, correct = 0, prediction = -1;
  float predictionVal = 0.0;
  for (std::vector<float>::iterator it = outputs.begin(); it != outputs.end(); ++it) {
    if (*it > predictionVal) {
      predictionVal = *it;
      prediction = node;
    }
    node++;
  }
  correct = (prediction == actualOutput) ? 1 : -1;
  return correct;
}

// Print prediction - returns 1 if correct, -1 if incorrect, 0 if no actualOutput given
int Network::testAndPrintResults(std::vector<float> outputs, int actualOutput) {
  std::cout << "Test results: \n";
  int node = 0, correct = 0, prediction = -1;
  float predictionVal = 0.0;
  for (std::vector<float>::iterator it = outputs.begin(); it != outputs.end(); ++it) {
    std::cout << "  " << node << " = " << *it << std::endl;
    if (*it > predictionVal) {
      predictionVal = *it;
      prediction = node;
    }
    node++;
  }
  std::cout << "  Predicted: " << prediction;
  if (actualOutput >= 0) {
    std::cout << " Actual: " << actualOutput;
    correct = (prediction == actualOutput) ? 1 : -1;
  }
  std::cout << std::endl;
  return correct;
}

// Test network on a set of inputs and target outputs
int Network::test(std::vector<float> inputs, int output, bool print) {
  // Loop forward
  for (std::vector<Layer>::iterator it = layers.begin(); it != layers.end(); ++it) {
    inputs = it->feedForward(inputs);
  }
  // Final inputs are actual outputs
  return (print == true) ? testAndPrintResults(inputs, output) : testResults(inputs, output);
}

// Get prediction from network from a set of inputs
std::vector<float> Network::predict(std::vector<float> inputs) {
  // Loop forward
  for (std::vector<Layer>::iterator it = layers.begin(); it != layers.end(); ++it) {
    inputs = it->feedForward(inputs);
  }
  // Final inputs are actual outputs
  return inputs;
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

// Initialize Network Layers
void Network::initNetwork(std::vector< std::vector<float> > weights)
{
  // Initialize random seed
  srand(time(NULL));

  // Initialize input layer
  Layer layer = Layer(getNumInputNodes(), 0, std::vector<float>());
  layers.push_back(layer);

  int l = 0;
  // Initialize hidden layers
  for (int hid = 0; hid < getNumHiddenLayers(); hid++) {
    if (hid == 0) { 
      layer = Layer(getNumHiddenNodes(), getNumInputNodes(), weights[l]);
    }
    else {
      layer = Layer(getNumHiddenNodes(), getNumHiddenNodes(), weights[l]);
    }
    layers.push_back(layer);
    l++;
  }

  // Initialize output leyer
  if (getNumHiddenNodes() == 0) {
    layer = Layer(getNumOutputNodes(), getNumInputNodes(), weights[l]);
  }
  else {
    layer = Layer(getNumOutputNodes(), getNumHiddenNodes(), weights[l]);
  }
  layers.push_back(layer);
}

// Public constructor - New network 
Network::Network(int in, int hid, int out, int lay) {
  numInputNodes = in;
  numHiddenNodes = hid;
  numOutputNodes = out;
  numHiddenLayers = lay;
  initNetwork(std::vector< std::vector<float> > (numHiddenLayers + 1, std::vector<float>()));
}

// Public constructor - Network from weight file
Network::Network(std::string weightFile) {
  WeightFile wFile = WeightFile(weightFile);
  numInputNodes = wFile.getNumInputNodes(); 
  numHiddenNodes = wFile.getNumHiddenNodes();
  numOutputNodes = wFile.getNumOutputNodes();
  numHiddenLayers = wFile.getNumHiddenLayers();
  std::vector< std::vector<float> > weights = wFile.getWeights();
  initNetwork(weights);
}
