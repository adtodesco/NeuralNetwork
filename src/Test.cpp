#include "Network.h"

int main(int argc, char* argv[]) {
  Network myNet = Network(argv[1]);
  myNet.writeWeightFile("./files/weights/");
}
