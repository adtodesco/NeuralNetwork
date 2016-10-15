import random
import math

def writeTrainingFile(function):
  for i in range(20):
    if function == 'sine':
       temp = random.uniform(0, 9)
       target = open(filename, 'w')
       print("%s %s" % (temp, round(math.sin(temp))))

if __name__ == '__main__':
  writeTrainingFile('sine')
