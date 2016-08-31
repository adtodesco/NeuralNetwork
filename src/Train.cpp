#include <iostream>
#include <vector>

#include "Network.h"

int main()
{
  Network myNeuralNet = Network(2, 2, 2, 1);
  //Network myNeuralNet = Network(10, 5, 7, 1);
  //Network myNeuralNet = Network(5, 5, 2, 1);

  static const float inputArr[] = {-0.5, 0.5};
  //static const float inputArr[] = {1.0,2.5,3.5,4.2,4.3,1.5,2.5,1.9,-1.0,-2.6};
  //static const float inputArr[] = {1.0,2.5,3.5,4.5,1.5};
  static const float outputArr[] = {5.0, 10.0};
  std::vector<float> inputVec (inputArr, inputArr + sizeof(inputArr) / sizeof(inputArr[0]) );
  std::vector<float> outputVec (outputArr, outputArr + sizeof(outputArr) / sizeof(outputArr[0]) );

  for (int it = 0; it < 10; it++) {
    myNeuralNet.train(inputVec, outputVec);
  }
  return 0;
}
