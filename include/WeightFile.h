#ifndef WEIGHTFILE_H
#define WEIGHTFILE_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream> 

#include "Constants.h"

class WeightFile
{
private:
  int parseHeader(std::string line);
  std::vector<float> parseWeights(std::string line);
  bool fileExists(std::string filename);
  std::string intToString(int i);
  void testWeightFile();

  int numInputNodes, numHiddenNodes, numOutputNodes, numHiddenLayers;
  std::vector< std::vector<float> > weights;
  bool weightFileSet; 
public:
  WeightFile(std::string weightFile);
  WeightFile(int in, int hid, int out, int lay, std::vector< std::vector<float> > wei); 

  void setWeightFile(std::string weightFile);
  void writeWeightFile(std::string weightsDir, std::string baseName);

  int getNumInputNodes();
  int getNumHiddenNodes();
  int getNumOutputNodes();
  int getNumHiddenLayers();
  std::vector< std::vector<float> > getWeights();
};

#endif
