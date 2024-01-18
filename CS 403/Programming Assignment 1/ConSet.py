# Fill the class given below for the first part of the assignment
# You can add new functions if necessary
# but don't change the signatures of the ones included

# This solution is implemented by
#       Ataollah Hosseinzadeh Fard      ID: 28610

from threading import Lock, Semaphore
import random

class ConSet:
    def __init__(self):
        self.set = dict()
        self.mtx = Lock()
        self.sem = Semaphore(0)

    def insert(self, newItem):
        self.mtx.acquire()
        if newItem not in self.set or self.set[newItem] == False:
            self.set[newItem] = True
            self.sem.release()
        self.mtx.release()
    
    def pop(self):
        res = None
        
        self.sem.acquire()
        self.mtx.acquire()
        keys = list(self.set.keys())
        random.shuffle(keys)
        for k in keys:
            if self.set[k] == True:
                self.set[k] = False
                res = k
                break
        self.mtx.release()

        return res
    
    def printSet(self):
        self.mtx.acquire()
        items = [str(k) for k, v in self.set.items() if v == True]
        print(", ".join(map(str, items)))
        self.mtx.release()
