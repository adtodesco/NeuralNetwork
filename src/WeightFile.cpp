#include "WeightFile.h"

// Writes neural network weights to file
void WeightFile::writeWeightFile(std::string weightFile) {
  
  // Get time information for metadata
  time_t t = time(0);
  struct tm * now = localtime( & t);
  std::string ymd = std::to_string(now->tm_year + 1900) + '-' + std::to_string(now->tm_mon + 1) + '-' + std::to_string(now->tm_mday);
  std::string hms = std::to_string(now->tm_hour) + ':' + std::to_string(now->tm_min) + ':' + std::to_string(now->tm_sec);

  // Write weight file
  std::ofstream weightFileStream;
  weightFileStream.open(weightFile);
  weightFileStream << "Neural Network Weight File: " << weightFile << std::endl;
  weightFileStream << "Created: " << ymd << ' ' << hms << std::endl;
  weightFileStream << "Input Nodes: " << getNumInputNodes() << std::endl;
  weightFileStream << "Hidden Nodes: " << getNumHiddenNodes() << std::endl;
  weightFileStream << "Output Nodes: " << getNumOutputNodes() << std::endl;
  weightFileStream << "Hidden Layers: " << getNumHiddenLayers() << std::endl; 
  for (std::vector< std::vector<float> >::iterator node = weights.begin(); node != weights.end(); ++node) {
    for (std::vector<float>::iterator pnode = node->begin(); pnode != node->end(); ++pnode) {
      weightFileStream << *pnode << ',';
    }
    weightFileStream << std::endl;
  }
  weightFileStream.close();
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

  while (std::getline(wFile, val)) {
    weights.push_back(parseWeights(val)); 
  }
}

// Public constructor
WeightFile::WeightFile(std::string weightFile) {
  setWeightFile(weightFile);
}

// Public constructor 
WeightFile::WeightFile(int in, int hid, int out, int lay, std::vector< std::vector<float> > wei) {
  numInputNodes = in;
  numHiddenNodes = hid;
  numOutputNodes = out;
  numHiddenLayers = lay;
  weights = wei;
}
