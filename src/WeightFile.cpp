#include "WeightFile.h"

// Check for file existance
bool WeightFile::fileExists(std::string filename) {
  std::ifstream f(filename);
  return f.good();
}

// Return 3-digit string from integer
std::string WeightFile::intToString(int i) {
  if (i < 10) {
    return "00" + std::to_string(i);
  } else if (i < 100) {
    return "0" + std::to_string(i);
  }
  return std::to_string(i);
}

// Writes neural network weights to file
void WeightFile::writeWeightFile(std::string weightsDir, std::string baseName) {
  // Check that filename is unique.  Append tag if already exists
/*  int identifier = 1;
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
  weightFile.close();*/
}

// Confirm that the weightFile has been set
void WeightFile::testWeightFile() {
  if (weightFileSet == false) {
    std::cerr << "  Error: Weight file not set\n";
    exit(1);
  }
}

// Return number of input nodes
int WeightFile::getNumInputNodes() {
  testWeightFile();
  return numInputNodes;
}

// Return number of hidden nodes
int WeightFile::getNumHiddenNodes() {
  testWeightFile();
  return numHiddenNodes;
}

// Return number of output nodes
int WeightFile::getNumOutputNodes() {
  testWeightFile();
  return numOutputNodes;
}

// Return number of hidden layers
int WeightFile::getNumHiddenLayers() {
  testWeightFile();
  return numHiddenLayers;
}

// Return weights
std::vector< std::vector<float> > WeightFile::getWeights() {
  testWeightFile();
  return weights;
}

// Parses lines of weight file header to get metadata
int WeightFile::parseHeader(std::string line) {
  size_t index = line.find(':');
  return std::stoi(line.substr(index + 2, line.size() - 1));
}

// Parses lines of weight file weights to return vector
std::vector<float> WeightFile::parseWeights(std::string line) {
  std::vector<float> weights;
  while (line.size() > 0) {
    size_t index = line.find(',');
    weights.push_back(std::stof(line.substr(0, index)));
    line = line.substr(index + 1, line.size() - 1);
  }
  return weights;
}

// Set weight file and update instance variables
void WeightFile::setWeightFile(std::string weightFile) {
  std::ifstream wFile(weightFile);
  if (!wFile.good()) {
    std::cerr << "  ERROR: Weight file " << weightFile << " is "
      "unreadable or does not exist\n";
    exit(1);
  }

  std::string val;
  std::getline(wFile, val);
  std::getline(wFile, val);
  std::getline(wFile, val);
  numInputNodes = parseHeader(val);
  std::getline(wFile, val);
  numHiddenNodes = parseHeader(val);
  std::getline(wFile, val);
  numOutputNodes = parseHeader(val);
  std::getline(wFile, val);
  numHiddenLayers = parseHeader(val);

  std::vector< std::vector<float> > weights;
  while (std::getline(wFile, val)) {
    weights.push_back(parseWeights(val)); 
  }
}

// Public constructor
WeightFile::WeightFile(std::string weightFile) {
  if (!weightFile.empty()) {
    setWeightFile(weightFile);
    weightFileSet = true;
  }
  else {
    weightFileSet = false;
  } 
}
