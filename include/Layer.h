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

  int numNodes, numPrevNodes;
  float getWeightedInput(std::vector<float> inputs, std::vector<Link> inputLinks);
  std::vector<float> calculateLinkDeltas();
  void calculateNodeDeltas(std::vector<float> linkDeltas);

public:
  Layer(int numNodes, int numPrevNodes);

  void setNumNodes(int nNodes) { numNodes = nNodes; }
  void setNumPrevNodes(int nPrevNodes) { numPrevNodes = nPrevNodes; }

  // Do I want getters/setters for private vectors?
  // std::vector<Node> getNodes() { return nodes; }
  // std::vector< std::vector<Link> > getLinks() { return links; }
  int getNumNodes() { return numNodes; }
  int getNumPrevNodes() { return numPrevNodes; }

  std::vector<float> feedForward(std::vector<float> inputs); 
  std::vector<float> backPropegation(std::vector<float> linkDeltas);

  void printNodes();
  void printLinks();
};

#endif
