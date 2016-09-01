#include "Link.h"

// Set weight instance variable
void Link::updateWeight(float delta, float input) {
  setWeight(getWeight() - LEARNINGRATE * delta * input); 
}
