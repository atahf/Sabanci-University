"""
    Ataollah Hosseinzadeh Fard
    ID: 28610
    CS301 Assignment 4

    for the result part of report, the partC function is used
"""


import time
import random
import matplotlib.pyplot as plt

#   beggining of part (a)

def createRandomMatrix(n, m):
    return [[random.randint(0, 1) for j in range(m)] for i in range(n)]

def optimalPath(farm, i, j):
    if i == 0 and j == 0:
        return " (" + str(i+1) + ", " + str(j+1) + ") "
    if i == 0 and j > 0:
        return optimalPath(farm, i , j-1) + "-> (" + str(i+1) + ", " + str(j+1) + ") "#
    elif i > 0 and j == 0:
        return optimalPath(farm, i-1 , j) + "-> (" + str(i+1) + ", " + str(j+1) + ") "#
    elif i > 0 and j > 0:
        if(farm[i-1][j] > farm[i][j-1]):
            return optimalPath(farm, i-1, j) + "-> (" + str(i+1) + ", " + str(j+1) + ") "#
        elif(farm[i-1][j] < farm[i][j-1]):
            return optimalPath(farm, i, j-1) + "-> (" + str(i+1) + ", " + str(j+1) + ") "#
        else:
            upper = (farm[i-2][j] if i > 1 else -1)
            lower = (farm[i][j-2] if i > 1 else -1)
            if upper < lower:
                return optimalPath(farm, i, j-1) + "-> (" + str(i+1) + ", " + str(j+1) + ") "#
            if upper > lower:
                return optimalPath(farm, i-1, j) + "-> (" + str(i+1) + ", " + str(j+1) + ") "#
            else:
                if i > j:
                    return optimalPath(farm, i-1, j) + "-> (" + str(i+1) + ", " + str(j+1) + ") "#
                else:
                    return optimalPath(farm, i, j-1) + "-> (" + str(i+1) + ", " + str(j+1) + ") "#

def getOptimalPath(matrix):
    row, col = len(matrix), len(matrix[0])
    valueMatrix = [[i+j+matrix[i][j] for j in range(col)] for i in range(row)]
    return optimalPath(valueMatrix, row-1, col-1)

def benchmarkLinear(startN, endN, stepN, M, innerTestCount):
    complexities = []
    seconds = []
    currentN = startN
    while currentN <= endN:
        complexity = currentN * M
        complexities.append(complexity)
        times = 0
        for _ in range(innerTestCount):
            tmpMatrix = createRandomMatrix(currentN, M)
            t0 = time.time()
            print(getOptimalPath(tmpMatrix))
            t1 = time.time()
            times += (t1 - t0)
        seconds.append(times / innerTestCount)
        currentN += stepN
    plt.plot(complexities, seconds)
    plt.ylabel('Execution Time (s)')
    plt.xlabel('Input size (n x m)')
    plt.show()
    
#   end of part (a)

def benchmarkSquare(start, end, step, innerTestCount):
    complexities = []
    seconds = []
    current = start
    while current <= end:
        complexity = current * current
        complexities.append(complexity)
        times = 0
        for _ in range(innerTestCount):
            tmpMatrix = createRandomMatrix(current, current)
            t0 = time.time()
            print(getOptimalPath(tmpMatrix))
            t1 = time.time()
            times += (t1 - t0)
        seconds.append(times / innerTestCount)
        current += step
    plt.plot(complexities, seconds)
    plt.ylabel('Execution Time (s)')
    plt.xlabel('Input size (n x m)')
    plt.show()

#   this function is for part (b)
# only used 1 test case from assignment pdf (image)
def partB():
    tc1 = [[1,0,1,0,0,0],
           [0,1,0,1,0,0],
           [0,1,1,0,0,0],
           [0,0,0,0,1,0],
           [1,0,1,0,0,1],]
    t0 = time.time()
    print(getOptimalPath(tc1))
    print(time.time() - t0, "seconds")

#   this function is for part (c)
def partC():
    benchmarkLinear(50, 200, 25, 50, 100)
    benchmarkSquare(50, 200, 25, 100)
