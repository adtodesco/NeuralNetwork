#include "Node.h"

// Set node value and output instance variables
void Node::setValue(float v) { 
  value = v;
  output = 1 / (1 + std::exp(v * -1));
}
