import random
import sys

assert(len(sys.argv) == 6)

width = int(sys.argv[1])
height = int(sys.argv[2])
l_bound = float(sys.argv[3])
u_bound = float(sys.argv[4])
fname = sys.argv[5]

file = open(fname,"w+")

for i in range(0,width):
    for j in range(0,height):
        num = random.uniform(l_bound,u_bound)
        file.write(str(num))
        if j < height - 1:
            file.write(',')
    file.write('\n')

file.close()
