#include "Node.h"

//Node::Node(float v)
//{
//  setValue(v);
//}

void Node::setValue(float v) { 
  value = v;
  output = 1 / (1 + std::exp(v));
}

/*
void Node::addIncoming(Link l)
{
  incoming.push_back(l);
}

void Node::addOutgoing(Link l)
{
  outgoing.push_back(l);
}
*/
