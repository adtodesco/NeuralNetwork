#include "Layer.h"

Layer::Layer(int numNodes, int numPrevNodes)
{
  setNumNodes(numNodes);
  setNumPrevNodes(numPrevNodes);
  // Create numNodes + 1 nodes to include bias node
  // Maybe this is wrong... if the bias node value doesn't change, and only
  // the weights change, just make the bias node value a constant and add a link.
  // This way, there won't need to be another case here for input layer nodes vs. other layers
  for (int n = 0; n < numNodes; n++) {
    Node newNode = Node(0);
    nodes.push_back(newNode);
  }
  for (int n = 0; n < numNodes; n++) {
    std::vector<Link> newVector;
    links.push_back(newVector);
    // Push back numPrevNodes + 1 to incorperate bias node
    for (int pn = 0; pn < (numPrevNodes + 1); pn++) {
      Link newLink = Link(INITIALWEIGHT);
      links[n].push_back(newLink);
    }
  }
}

float Layer::getWeightedInput(std::vector<float> inputs, std::vector<Link> inputLinks) {
  float sum = 0.0;
  for (int pn = 0; pn < getNumPrevNodes(); pn++) {
    // std::cout << "Input: " << inputs[pn] << " Weight: " << inputLinks[pn].getWeight() << '\n';
    sum = sum + inputs[pn] * inputLinks[pn].getWeight();
  }
  // std::cout << "Input: " << BIASINPUT << " Weight: " << inputLinks[getNumPrevNodes()].getWeight() << '\n';
  sum = sum + BIASINPUT * inputLinks[getNumPrevNodes()].getWeight();
  return sum;
}

std::vector<float> Layer::feedForward(std::vector<float> inputs) {
  // Update these nodes based on input nodes and return
  // printLinks();
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

// Calculate the delta values for each node
void Layer::updateDeltas(std::vector<float> deltinis) {
  for (int n = 0; n < getNumNodes(); n++) {
    nodes[n].setDelta(deltinis[n] * nodes[n].getOutput() * (1 - nodes[n].getOutput()));
  }
}

// Calculate the summed-weighted-delta values for each link
std::vector<float> Layer::getDeltinis() {
  std::vector<float> summedDeltas;
  for (int pn = 0; pn < getNumPrevNodes() + 1; pn++) {
    float summedDelta = 0.0;
    for (int n = 0; n < getNumNodes(); n++) {
      summedDelta = summedDelta + links[pn][n].getWeight() * nodes[n].getDelta(); 
    }
    summedDeltas.push_back(summedDelta);
  }
  return summedDeltas; 
}

std::vector<float> Layer::backPropegation(std::vector<float> deltinis) {
    // getSummedDeltas (deltas)
    // calculateCurrentDelta
    // updateCurrentDelta
    updateDeltas(deltinis);

    // multiply by input to get partial deriv wrt weight

    // update the weight

    // Return vector of summed-weighted-deltas for next iteration
    return getDeltinis(); 
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
