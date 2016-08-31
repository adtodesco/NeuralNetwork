#ifndef LAYER_H
#define LAYER_H

#include <iostream>
#include <vector>
#include "Node.h"
#include "Link.h"
#include "Constants.h"

class Layer
{
private:
  std::vector<Node> nodes;
  std::vector< std::vector<Link> > links;
  std::vector<float> inputs;

  int numNodes, numPrevNodes;

  void updateDeltas(std::vector<float> deltinis);
  void updateInputs(std::vector<float> in) { inputs = in; }
  void updateWeights();

  float getWeightedInput(std::vector<Link> inputLinks);
  std::vector<float> getDeltinis();
  std::vector<float> getInputs() { return inputs; }

public:
  Layer(int numNodes, int numPrevNodes);

  void setNumNodes(int nNodes) { numNodes = nNodes; }
  void setNumPrevNodes(int nPrevNodes) { numPrevNodes = nPrevNodes; }

  // Do I want getters/setters for private vectors?
  // std::vector<Node> getNodes() { return nodes; }
  // std::vector< std::vector<Link> > getLinks() { return links; }
  int getNumNodes() { return numNodes; }
  int getNumPrevNodes() { return numPrevNodes; }

  std::vector<float> feedForward(std::vector<float> in); 
  std::vector<float> backPropegation(std::vector<float> deltinis);

  void printNodes();
  void printLinks();
};

#endif
