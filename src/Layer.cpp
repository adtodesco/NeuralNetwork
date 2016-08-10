#include "Layer.h"

Layer::Layer(int numNodes, int numPrevNodes)
{
  setNumNodes(numNodes);
  setNumPrevNodes(numPrevNodes);
  // Create numNodes + 1 nodes to include bias node
  // Maybe this is wrong... if the bias node value doesn't change, and only
  // the weights change, just make the bias node value a constant and add a link.
  // This way, there won't need to be another case here for input layer nodes vs. other layers
  for (int n = 0; n < numNodes + 1; n++) {
    Node newNode = Node(0);
    nodes.push_back(newNode);
  }
  for (int n = 0; n < numNodes; n++) {
    std::vector<Link> newVector;
    links.push_back(newVector);
    for (int p = 0; p < numPrevNodes; p++) {
      Link newLink = Link(INITIALWEIGHT);
      links[n].push_back(newLink);
    }
  }
}

float Layer::getWeightedInput(std::vector<float> inputs, std::vector<Link> weights) {
  float sum = 0.0;
  for (int pn = 0; pn < getNumPrevNodes(); pn++) {
    std::cout << "Input: " << inputs[pn] << " Weight: " << weights[pn].getWeight() << '\n';
    sum = sum + inputs[pn] * weights[pn].getWeight();
  }
  return sum;
}

std::vector<float> Layer::feedForward(std::vector<float> inputs) {
  // Update these nodes based on input nodes and return
  printLinks();
  std::vector<float> outputs;
  // Set outputs to inputs if input layer
  if (getNumPrevNodes() == 0) {
    outputs = inputs;
  }
  else {
    for (int n = 0; n < getNumNodes(); n++) { 
      float weightedInput = getWeightedInput(inputs, links[n]);
      nodes[n].setValue(weightedInput);
      outputs.push_back(nodes[n].getOutput());
      std::cout << "Weighted input for Node " << n << " = " << weightedInput << '\n';
    }
  }
  // Return outputed values from nodes
  return outputs;
}

void Layer::printNodes()
{
  for (std::vector<Node>::iterator it = nodes.begin() ; it != nodes.end(); ++it) {
    std::cout << " Node: " << it - nodes.begin() << " = " << it->getValue() << '\n';
  }
}

void Layer::printLinks()
{
  for (int n = 0; n < getNumNodes(); n++) {
    for (int pn = 0; pn < getNumPrevNodes(); pn++) {
      std::cout << " Link: " << pn << "->" << n << " = " << links[n][pn].getWeight() << '\n';
    }
  }
}
