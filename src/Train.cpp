#include <iostream>
#include <vector>

#include "Network.h"

int main()
{
  Network myNeuralNet = Network(5, 10, 2, 1);

  static const float inputArr[] = {1.0,2.5,3.5,4.5,1.5};
  static const float outputArr[] = {5.0, 12.0};
  std::vector<float> inputVec (inputArr, inputArr + sizeof(inputArr) / sizeof(inputArr[0]) );
  std::vector<float> outputVec (outputArr, outputArr + sizeof(outputArr) / sizeof(outputArr[0]) );

  myNeuralNet.train(inputVec, outputVec);
  return 0;
}
