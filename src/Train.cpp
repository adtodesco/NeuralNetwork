#include <iostream>
#include <vector>

#include "Network.h"

int main()
{
  // Initialize random seed
  srand(time(NULL));

  Network myNeuralNet = Network(2, 2, 2, 1);
  //Network myNeuralNet = Network(10, 5, 7, 1);
  //Network myNeuralNet = Network(5, 5, 2, 1);

  static const float inputArr[] = {-10, -3.0};
  //static const float inputArr[] = {1.0,2.5,3.5,4.2,4.3,1.5,2.5,1.9,-1.0,-2.6};
  //static const float inputArr[] = {1.0,2.5,3.5,4.5,1.5};
  static const float outputArr[] = {0.99, 0.01};
  std::vector<float> inputVec (inputArr, inputArr + sizeof(inputArr) / sizeof(inputArr[0]) );
  std::vector<float> outputVec (outputArr, outputArr + sizeof(outputArr) / sizeof(outputArr[0]) );

  float totalError = 1.0;
  float prevTotalError = totalError;
  int iterations = 0;
  while ((totalError > 0.0001) && (totalError <= prevTotalError)) {
  //for (int i = 0; i < 10; i++) {
    prevTotalError = totalError;
    iterations++;
    std::cout << "===============\n";
    std::cout << "Iteration: " << iterations << '\n';
    std::cout << "===============\n";
    totalError = myNeuralNet.train(inputVec, outputVec);
    std::cout << "totalError: " << totalError << " prevTotalError " << prevTotalError << '\n';
  }
  return 0;
}
