#ifndef CONNECTION_H
#define CONNECTION_H

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
