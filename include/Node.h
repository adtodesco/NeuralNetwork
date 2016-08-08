#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <cmath>

class Node
{
private:
  float value;

//  std::vector<Link> incoming;
//  std::vector<Link> outgoing;

public:
  Node(float v) { setValue(v); }

//  void addIncoming(Link c);
//  void addOutgoing(Link c);
  float getOutput() { return 1 / (1 + std::exp(getValue())); }
  float getValue() { return value; }
  void setValue(float v) { value = v; }
};

#endif
