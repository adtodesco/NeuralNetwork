import random
import math
from subprocess import call
import os

trainDir = "./files/train/"
testDir = "./files/tests/"
weightsDir = "./files/weights/"
functions = ["sine"]

def writeTempFiles(function):
  trainTarget = open(trainDir + function + '.csv', 'w')
  testTarget = open(testDir + function + '.csv', 'w')

  for i in range(10000):
    if function == "sine":
      temp = random.uniform(-1.57, 1.57)
      trainTarget.write("%s,%s\n" % (temp, int(round(math.sin(temp) + 1))))
      temp = random.uniform(-1.57, 1.57)
      testTarget.write("%s,%s\n" % (temp, int(round(math.sin(temp) + 1))))

def removeTempFiles(function):
  os.remove(trainDir + function + '.csv')
  os.remove(testDir + function + '.csv')
  os.remove(weightsDir + function + "-001.csv")

def trainNetwork(function):
    call(["./bin/annet",
        "train",
        "./" + trainDir + function + ".csv", 
        "-w", function, 
        "-i", "1",
        "-o", "3", 
        "-h", "5",
        "-l", "1"])

def testNetwork(function):
    call(["./bin/annet",
        "test",
        "./" + testDir + function + ".csv",
        weightsDir + function + "-001.csv"])

if __name__ == '__main__':
  for func in functions:
    writeTempFiles(func)
    trainNetwork(func)
    testNetwork(func) 
    removeTempFiles(func)

  print "Script dir: " + os.path.dirname(os.path.abspath(__file__))
  print "CWD:        " + os.getcwd()
