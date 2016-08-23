#ifndef LINK_H
#define LINK_H

#include "Constants.h"

class Link
{
private:
  float weight, delta;

public:
  Link(float w) { setWeight(w); }

  float getWeight() { return weight; }
  float getDelta() { return delta; }
  void setWeight(float w) { weight = w; }
  void setDelta(float d) { delta = d; }
  void updateWeight(float delta, float input);
};

#endif
