import random
import math

def writeTempFiles(function):
  trainDir = "./files/train/"
  testDir = "./files/tests/"

  trainTarget = open(trainDir + function + '.csv', 'w')
  testTarget = open(testDir + function + '.csv', 'w')

  for i in range(20):
    if function == 'sine':
       temp = random.uniform(0, 9)
       trainTarget.write("%s,%s,\n" % (temp, int(round(math.sin(temp) + 1))))
       temp = random.uniform(0, 9)
       testTarget.write("%s,%s,\n" % (temp, int(round(math.sin(temp) + 1))))

if __name__ == '__main__':
  writeTempFiles('sine')
  
