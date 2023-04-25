import time
import matplotlib.pyplot as plt
import statistics

data = []

f = open("dna.txt", "r")
for x in f:
    data.append(x)
  
data1, data2 = data[:30], data[30:]

data1.append("xxxxxxxxxxxxxxxxxxxxxxxxx")
data2.append("yyyyyyyyyyyyyyyyyyyyyyyyy")

est = [[], [], [], [], []]
test = [5, 10, 15, 20, 25]
print('Recursive\n')
for j in range(31):
    for i in range(3):
        st = time.time()

        def lcs(X,Y,i,j):
            if (i == 0 or j == 0):
                return 0
            elif X[i-1] == Y[j-1]:
                return 1 + lcs(X,Y,i-1,j-1)
            else:
                return max(lcs(X,Y,i,j-1),lcs(X,Y,i-1,j))

        X = data1[j]
        Y = data2[j]
        
        lX = len(X)
        lY = len(Y)


        #print("Length of LCS is ", lcs(X,Y,lX,lY))
        print("Length of LCS is ", lcs(X,Y,test[i],test[i]))

        et = time.time()

        et = time.time()

        elapsed_time = et - st
        print('Execution time:', elapsed_time, 'seconds', 'for test', test[i])
        est[i].append(elapsed_time)

means = [statistics.mean(est[i][:30]) for i in range(3)]
stds = [statistics.stdev(est[i][:30]) for i in range(3)]

worst = [est[i][-1] for i in range(3)]

print()
print(worst)
print(means)
print(stds)

figure, axis = plt.subplots(2, 1)
plt.subplots_adjust(hspace=1)

axis[0].plot(test[:3], means)
axis[0].set_title("Average case")

axis[1].plot(test[:3], worst)
axis[1].set_title("Worst case")

plt.show()


