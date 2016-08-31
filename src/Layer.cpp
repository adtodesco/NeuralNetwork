#include "Layer.h"

Layer::Layer(int numNodes, int numPrevNodes)
{
  setNumNodes(numNodes);
  setNumPrevNodes(numPrevNodes);
  for (int n = 0; n < numNodes; n++) {
    Node newNode = Node(0);
    nodes.push_back(newNode);
  }
  // Skip adding links to input layer
  if (numPrevNodes > 0) {
    for (int n = 0; n < numNodes; n++) {
      std::vector<Link> newVector;
      links.push_back(newVector);
      // Push back numPrevNodes + 1 to incorperate bias node
      for (int pn = 0; pn < numPrevNodes + 1; pn++) {
	Link newLink = Link(INITIALWEIGHT);
	links[n].push_back(newLink);
      }
    }
  }
}

float Layer::getWeightedInput(std::vector<Link> inputLinks) {
  float sum = 0.0;
  for (int pn = 0; pn < getNumPrevNodes(); pn++) {
    // std::cout << "Input: " << getInputs()[pn] << " Weight: " << inputLinks[pn].getWeight() << '\n';
    sum = sum + getInputs()[pn] * inputLinks[pn].getWeight();
  }
  // std::cout << "Input: " << BIASINPUT << " Weight: " << inputLinks[getNumPrevNodes()].getWeight() << '\n';
  sum = sum + BIASINPUT * inputLinks[getNumPrevNodes()].getWeight();
  return sum;
}

std::vector<float> Layer::feedForward(std::vector<float> in) {
  updateInputs(in);
  // Update these nodes based on input nodes and return
  // printLinks();
  std::vector<float> outputs;
  // Set outputs to inputs if input layer
  if (getNumPrevNodes() == 0) {
    outputs = getInputs();
  }
  else {
    for (int n = 0; n < getNumNodes(); n++) { 
      float weightedInput = getWeightedInput(links[n]);
      nodes[n].setValue(weightedInput);
      outputs.push_back(nodes[n].getOutput());
      //std::cout << "Weighted input for Node " << n << " = " << weightedInput << '\n';
    }
  }
  // Return outputed values from nodes
  return outputs;
}

// Calculate and update delta values for each node from next layer deltinis
void Layer::updateDeltas(std::vector<float> deltinis) {
  for (int n = 0; n < getNumNodes(); n++) {
    nodes[n].setDelta(deltinis[n] * nodes[n].getOutput() * (1 - nodes[n].getOutput()));
  }
}

// Calculate and update weight values for each link
void Layer::updateWeights() {
  for (int n = 0; n < getNumNodes(); n++) {
    for (int pn = 0; pn < getNumPrevNodes() + 1; pn++) {
      links[n][pn].updateWeight(nodes[n].getDelta(), getInputs()[pn]);
    }
  } 
}

// Calculate the summed-weighted-delta values for each link
std::vector<float> Layer::getDeltinis() {
  std::vector<float> summedDeltas;
  for (int pn = 0; pn < getNumPrevNodes() + 1; pn++) {
    float summedDelta = 0.0;
    for (int n = 0; n < getNumNodes(); n++) {
      summedDelta = summedDelta + links[n][pn].getWeight() * nodes[n].getDelta();
    }
    summedDeltas.push_back(summedDelta);
  }
  return summedDeltas; 
}

std::vector<float> Layer::backPropegation(std::vector<float> deltinis) {
    // Calculate and update delta values for each node from next layer deltinis
    updateDeltas(deltinis);

    // Calculate deltinis for use in previous layer delta calculations
    deltinis = getDeltinis();

    // Update weights after calculating deltinis to preserve original weights
    // in deltini calculation
    updateWeights();

    return deltinis; 
}

void Layer::printNodes()
{
  std::cout << "===========================\n";
  for (std::vector<Node>::iterator it = nodes.begin() ; it != nodes.end(); ++it) {
    std::cout << " Node: " << it - nodes.begin() << " = " << it->getValue() << '\n';
  }
}

void Layer::printLinks()
{
  std::cout << "===========================\n";
  for (int n = 0; n < getNumNodes(); n++) {
    for (int pn = 0; pn < getNumPrevNodes() + 1; pn++) {
      std::cout << " Link: " << pn << "->" << n << " = " << links[n][pn].getWeight() << '\n';
    }
  }
}
