#include <iostream>
#include <vector>

#include "Network.h"

int main()
{
  Network myNeuralNet = Network(5, 10, 2, 1);

  static const float arr[] = {1.0,2.5,3.5,4.5,1.5};
  std::vector<float> vec (arr, arr + sizeof(arr) / sizeof(arr[0]) );

  myNeuralNet.train(vec);
  return 0;
}
