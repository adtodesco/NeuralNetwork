#include "Network.h"

// Public constructor - New network 
Network::Network(int in, int hid, int out, int lay)
{
  numInputNodes = in;
  numHiddenNodes = hid;
  numOutputNodes = out;
  numHiddenLayers = lay;
  initNetwork();
}

// Public constructor - Network from weight file
Network::Network(std::string weightFile) {
  std::ifstream wFile(weightFile);
  if (!wFile.good()) {
    std::cerr << "  ERROR: Weight file " << weightFile << " is "
      "unreadable or does not exist\n";
    exit(1);
  }
  std::string val;
  std::getline(wFile, val);
  std::getline(wFile, val);
  std::getline(wFile, val, ':');
  std::getline(wFile, val);
  numInputNodes = std::stoi(val.substr(1, val.size() - 1));
  std::getline(wFile, val, ':');
  std::getline(wFile, val);
  numHiddenNodes = std::stoi(val.substr(1, val.size() - 1));
  std::getline(wFile, val, ':');
  std::getline(wFile, val);
  numOutputNodes = std::stoi(val.substr(1, val.size() - 1));
  std::getline(wFile, val, ':');
  std::getline(wFile, val);
  numHiddenLayers = std::stoi(val.substr(1, val.size() - 1));
  //while (!wFile.eof()) {
         
  //}
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

// Check for file existance
bool Network::fileExists(std::string filename) {
  std::ifstream f(filename);
  return f.good();
}

// Return 3-digit string from integer
std::string Network::intToString(int i) {
  if (i < 10) {
    return "00" + std::to_string(i);
  } else if (i < 100) {
    return "0" + std::to_string(i);
  }
  return std::to_string(i);
}

// Writes neural network weights to file
void Network::writeWeightFile(std::string weightsDir, std::string baseName) {
  // Check that filename is unique.  Append tag if already exists
  int identifier = 1;
  std::string filename = weightsDir + '/' + baseName + '-' + intToString(identifier) + ".csv"; 

  while (fileExists(filename) == true) {
    identifier++;
    filename = weightsDir + '/' + baseName + '-' + intToString(identifier) + ".csv"; 
  }
 
 // Get time information for metadata
  time_t t = time(0);
  struct tm * now = localtime( & t);
  std::string ymd = std::to_string(now->tm_year + 1900) + '-' + std::to_string(now->tm_mon + 1) + '-' + std::to_string(now->tm_mday);
  std::string hms = std::to_string(now->tm_hour) + ':' + std::to_string(now->tm_min) + ':' + std::to_string(now->tm_sec);

  // Write weight file
  std::ofstream weightFile;
  weightFile.open(filename);
  weightFile << "Neural Network Weight File: " << baseName + '-' + intToString(identifier) + ".csv" << std::endl;
  weightFile << "Created: " << ymd << ' ' << hms << std::endl;
  weightFile << "Input Nodes: " << getNumInputNodes() << std::endl;
  weightFile << "Hidden Nodes: " << getNumHiddenNodes() << std::endl;
  weightFile << "Output Nodes: " << getNumOutputNodes() << std::endl;
  weightFile << "Hidden Layers: " << getNumHiddenLayers() << std::endl; 
  std::vector< std::vector<Link> > links;
  for (std::vector<Layer>::iterator it = layers.begin() + 1; it != layers.end(); ++it) {
    links = it->getLinks();
    for (std::vector< std::vector<Link> >::iterator node = links.begin(); node != links.end(); ++node) {
      for (std::vector<Link>::iterator prevNode = node->begin(); prevNode != node->end(); ++prevNode) {
        weightFile << prevNode->getWeight() << ',';
      }
    }
    weightFile << std::endl;
  }
  weightFile.close();
}
