#ifndef LINK_H
#define LINK_H

#include "Constants.h"

class Link
{
private:
  float weight;

public:
  Link(float w) { setWeight(w); }

  float getWeight() { return weight; }
  void setWeight(float w) { weight = w; }
  void updateWeight(float delta, float input);
};

#endif
