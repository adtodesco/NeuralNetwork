#include "Link.h"

//Link::Link(float w)
//{
//  setWeight(w);
//}

void Link::updateWeight(float delta, float input) {
  setDelta(delta);
  setWeight(getWeight() * LEARNINGRATE * delta * input); 
}
