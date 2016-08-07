#ifndef LAYER_H
#define LAYER_H

#include <iostream>
#include <vector>
#include "Node.h"
#include "Link.h"

class Layer
{
private:
  std::vector<Node> nodes;
  std::vector< std::vector<Link> > links;

public:
  Layer(int numNodes, int numPrevNodes);

  std::vector<Node> getNodes() { return nodes; }
  std::vector< std::vector<Link> > getLinks() { return links; }

  std::vector<float> feedForward(std::vector<float> inputs); 

  void printNodes();
  void printLinks();
};

#endif
