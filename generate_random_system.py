#GENERATES A RANDOM DIAGONALLY DOMINANT MATRIX AND THE b VECTOR
import random
import sys

assert(len(sys.argv) == 5)

rows = int(sys.argv[1])
cols = rows
l_bound = float(sys.argv[2])
u_bound = float(sys.argv[3]) / 2
fname = sys.argv[4]


#GENERATE DIAGONALLY DOMINANT MATRIX
matrix = []
for i in range(0,rows):
    row = []
    row_sum = 0.0
    for j in range(0,cols):
        num = random.uniform(l_bound,u_bound)
        row += [num]
        row_sum += abs(num)
    matrix += [row]
    #set diag elem
    matrix[i][i] = row_sum + 1.0#random.uniform(row_sum + 1.0, row_sum * 2)


#TEST FOR DOMINANCE
for i in range(0,rows):
    summation = 0.0
    for j in range(0,cols):
        if i != j:
            summation += abs(matrix[i][j])
    assert abs(matrix[i][i]) >= summation

#ADD b
b = []
for i in range(0,rows):
    b.append(random.uniform(l_bound,u_bound))

#WRITE
file = open(fname,"w+")

for i in range(0,rows):
    for j in range(0,cols):
        file.write(str(matrix[i][j]))
        #if j < cols - 1:
        file.write(',')
    file.write(str(b[i]))
    file.write('\n')

file.close()
