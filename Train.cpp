#include <iostream>
#include <vector>

#include "Network.h"

int main()
{
  Network myNeuralNet = Network(5, 2, 10, 1);

  static const float arr[] = {1.0,2.0,3.0,4.0,5.0};
  std::vector<float> vec (arr, arr + sizeof(arr) / sizeof(arr[0]) );

  myNeuralNet.train(vec);
  return 0;
}
