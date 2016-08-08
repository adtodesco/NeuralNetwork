#ifndef LINK_H
#define LINK_H

class Link
{
private:
  float weight;

public:
  Link(float w);

  float getWeight() { return weight; }
  void setWeight(float w) { weight = w; }
};

#endif
