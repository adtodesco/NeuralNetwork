#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <cmath>

class Node
{
private:
  float value, output, delta;

public:
  Node(float v) { setValue(v); }

  float getOutput() { return output; }
  float getValue() { return value; }
  float getDelta() { return delta; }
  void setValue(float v);
  void setDelta(float d) { delta = d; }
};

#endif
