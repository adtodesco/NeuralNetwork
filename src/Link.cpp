#include "Link.h"

// Set weight instance variable
void Link::updateWeight(float delta, float input) {
  std::cout << "weight: " << getWeight() << " LEARNINGRATE: " << LEARNINGRATE << " delta: " << delta << " input: " << input << '\n';
  setWeight(getWeight() - LEARNINGRATE * delta * input); 
}
