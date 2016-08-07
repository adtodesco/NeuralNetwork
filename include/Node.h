#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
// #include "Link.h"

class Node
{
private:
  float value;

//  std::vector<Link> incoming;
//  std::vector<Link> outgoing;

public:
  Node(float v);

//  void addIncoming(Link c);
//  void addOutgoing(Link c);
  float getOutput();
  float getValue() { return value; }
  void setValue(float v) { value = v; }
};

#endif
