#include "Layer.h"

// Generate random float for initial weight
float Layer::randomFloat() {
  int randInt = rand() % (INITIALWEIGHTRANGE * 1000) - (INITIALWEIGHTRANGE * 500);
  float randFl = (float) randInt / 1000;
  return randFl;
}

// Public constructor
Layer::Layer(int numNodes, int numPrevNodes, std::vector<float> weights)
{
  setNumNodes(numNodes);
  setNumPrevNodes(numPrevNodes);
  // Initialize nodes
  for (int n = 0; n < numNodes; n++) {
    Node newNode = Node(0);
    nodes.push_back(newNode);
  }
  // Skip adding links to input layer
  if (numPrevNodes > 0) {
    int w = 0;
    // Initialize links
    for (int n = 0; n < numNodes; n++) {
      std::vector<Link> newVector;
      links.push_back(newVector);
      // Push back numPrevNodes + 1 to incorperate bias node
      for (int pn = 0; pn < numPrevNodes + 1; pn++) {
        if (weights.empty() == true) {
	  Link newLink = Link(randomFloat());
	  links[n].push_back(newLink);
        }
        else {
          Link newLink = Link(weights[w]);
          links[n].push_back(newLink);
          w++;
        }
      }
    }
  }
}

// Calculate the weighted input for node in layer 
float Layer::calculateWeightedInput(std::vector<Link> inputLinks) {
  float sum = 0.0;
  for (int pn = 0; pn < getNumPrevNodes(); pn++) {
    sum = sum + getInputs()[pn] * inputLinks[pn].getWeight();
  }
  sum = sum + BIASINPUT * inputLinks[getNumPrevNodes()].getWeight();
  return sum;
}

// Feed inputs through Network
std::vector<float> Layer::feedForward(std::vector<float> in) {
  // Update inputs vector instance variable
  setInputs(in);

  std::vector<float> outputs;
  // Outputs are equivilent to inputs in the first layer
  if (getNumPrevNodes() == 0) {
    outputs = getInputs();
  }
  else {
    // Set new values and obtain output for the input to the next layer
    for (int n = 0; n < getNumNodes(); n++) { 
      nodes[n].setValue(calculateWeightedInput(links[n]));
      outputs.push_back(nodes[n].getOutput());
    }
  }
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
    for (int pn = 0; pn < getNumPrevNodes(); pn++) {
      links[n][pn].updateWeight(nodes[n].getDelta(), getInputs()[pn]);
    }
    links[n][getNumPrevNodes()].updateWeight(nodes[n].getDelta(), BIASINPUT);
  } 
}

// Calculate the deltini for each node in the previous layer
std::vector<float> Layer::calculateDeltinis() {
  std::vector<float> deltinis;
  for (int pn = 0; pn < getNumPrevNodes() + 1; pn++) {
    float deltini = 0.0;
    for (int n = 0; n < getNumNodes(); n++) {
      deltini = deltini + links[n][pn].getWeight() * nodes[n].getDelta();
    }
    deltinis.push_back(deltini);
  }
  return deltinis; 
}

// Back propegation algorithm to update weights
std::vector<float> Layer::backPropegation(std::vector<float> deltinis) {
    // Calculate and update delta values for each node from next layer deltinis
    updateDeltas(deltinis);

    // Calculate deltinis for use in previous layer's delta calculations
    deltinis = calculateDeltinis();

    // Update weights after calculating deltinis to preserve original weights
    // in deltini calculation
    updateWeights();

    return deltinis; 
}

// Print node values in the layer
void Layer::printNodes()
{
  std::cout << "===========================\n";
  for (std::vector<Node>::iterator it = nodes.begin() ; it != nodes.end(); ++it) {
    std::cout << " Node: " << it - nodes.begin() << " = " << it->getValue() << std::endl;
  }
}

// Print link weights in the layer
void Layer::printLinks()
{
  std::cout << "===========================\n";
  for (int n = 0; n < getNumNodes(); n++) {
    for (int pn = 0; pn < getNumPrevNodes() + 1; pn++) {
      std::cout << " Link: " << pn << "->" << n << " = " << links[n][pn].getWeight() << std::endl;
    }
  }
}
