import random
import math
from subprocess import call
import os

# Directory locations
trainDir = "./files/train/"
testDir = "./files/tests/"
weightsDir = "./files/weights/"

# Functions in format name, input_nodes, output_nodes, hidden_nodes, hidden_layers
functions = [["sine", 1, 3, 5, 1]]

def writeTempFiles(function):
  trainTarget = open(trainDir + function[0] + '.csv', 'w')
  testTarget = open(testDir + function[0] + '.csv', 'w')

  for i in range(10000):
    if function[0] == "sine":
      temp = random.uniform(-1.57, 1.57)
      trainTarget.write("%s,%s\n" % (temp, int(round(math.sin(temp) + 1))))
      temp = random.uniform(-1.57, 1.57)
      testTarget.write("%s,%s\n" % (temp, int(round(math.sin(temp + 1)))))

def removeTempFiles(function):
  os.remove(trainDir + function[0] + '.csv')
  os.remove(testDir + function[0] + '.csv')
  os.remove(weightsDir + function[0] + "-001.csv")

def trainNetwork(function):
  call(["./bin/annet",
      "train",
      trainDir + function[0] + ".csv", 
      weightsDir + function[0] + "-weights.csv", 
      "-d",
      "-i", str(function[1]),
      "-o", str(function[2]), 
      "-h", str(function[3]),
      "-l", str(function[4])])

def testNetwork(function):
  call(["./bin/annet",
	"test",
	testDir + function[0] + ".csv",
	weightsDir + function[0] + "-weights.csv"])

if __name__ == '__main__':
  for func in functions:
    writeTempFiles(func)
    trainNetwork(func)
    testNetwork(func) 
    removeTempFiles(func)
