import random
import math
from subprocess import call
import os

trainDir = "./files/train/"
testDir = "./files/tests/"
weightsDir = "./files/weights/"
functions = [["sine", "1", "3"]]

def writeTempFiles(function):
  trainTarget = open(trainDir + function[0] + '.csv', 'w')
  testTarget = open(testDir + function[0] + '.csv', 'w')

  for i in range(10000):
    if function[0] == "sine":
      temp = random.uniform(-1.57, 1.57)
      trainTarget.write("%s,%s\n" % (temp, int(round(math.sin(temp) + 1))))
      temp = random.uniform(-1.57, 1.57)
      testTarget.write("%s,%s\n" % (temp, int(round(math.sin(temp) + 1))))

def removeTempFiles(function):
  os.remove(trainDir + function[0] + '.csv')
  os.remove(testDir + function[0] + '.csv')
  os.remove(weightsDir + function[0] + "-001.csv")

def trainNetwork(function):
  call(["./bin/annet",
      "train",
      "./" + trainDir + function[0] + ".csv", 
      "-w", function[0], 
      "-i", function[1],
      "-o", function[2], 
      "-h", "5",
      "-l", "1"])

def testNetwork(function):
  call(["./bin/annet",
	"test",
	"./" + testDir + function[0] + ".csv",
	weightsDir + function[0] + "-001.csv"])

if __name__ == '__main__':
  for func in functions:
    writeTempFiles(func)
    trainNetwork(func)
    testNetwork(func) 
    removeTempFiles(func)
