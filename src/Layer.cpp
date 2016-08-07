#include "Layer.h"

Layer::Layer(int numNodes, int numPrevNodes)
{
  for (int n = 0; n < numNodes; n++) {
    Node newNode = Node(0.5);
    nodes.push_back(newNode);
  }
  for (int p = 0; p < numPrevNodes; p++) {
    std::vector<Link> newVector;
    links.push_back(newVector);
    for (int n = 0; n < numNodes; n++) {
      Link newLink = Link(0.5);
      links[p].push_back(newLink);
    }
  }
}

std::vector<float> Layer::feedForward(std::vector<float> inputs) {
  // Update these nodes based on input nodes and return
  std::cout << "Feed forward\n";
  printNodes();
  printLinks();
  // Return outputed values from nodes
  return inputs;
}

void Layer::printNodes()
{
  for (std::vector<Node>::iterator it = nodes.begin() ; it != nodes.end(); ++it) {
    std::cout << " Node: " << it - nodes.begin() << " = " << it->getValue();
    std::cout << '\n';
  }
}

void Layer::printLinks()
{
  for (std::vector< std::vector<Link> >::iterator vit = links.begin() ; vit != links.end(); ++vit) {  
    for (std::vector<Link>::iterator lit = vit->begin() ; lit != vit->end(); ++lit) {
      std::cout << " Link: " << vit - links.begin() << "->" << lit - vit->begin() << " = " <<  lit->getWeight();
      std::cout << '\n';
    }
  }
}
