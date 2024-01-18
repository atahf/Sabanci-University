# This solution is implemented by
#       Ataollah Hosseinzadeh Fard      ID: 28610
# Please only change 'N' variable in this code for testing purposes

from ConSet import ConSet
from threading import Thread, Lock, Semaphore
import random

# Number of Nodes
N = 4

mtx = Lock()
count = Semaphore(0)
barrier = Semaphore(N)

def nodeWork(node_id, n):
    round_ = 1
    terminate = False

    while not terminate:
        barrier.acquire()

        tup = (random.randint(0, n*n), node_id)
        for mailbox in mailboxes:
            mailbox.insert(tup)

        maxId = []
        maxVal = -1
        for _ in range(n):
            data = mailboxes[node_id].pop()
            
            if data[0] > maxVal:
                maxVal = data[0]
                maxId = [data[1]]
            elif data[0] == maxVal:
                maxId.append(data[1])

            mtx.acquire()
            if data[1] == node_id:
                print(f"Node {node_id} proposes value {data[0]} for round {round_}.")
            mtx.release()

        mtx.acquire()
        if len(maxId) == 1:
            terminate = True
            print(f"Node {node_id} decided {maxId[0]} as the leader.")
        else:
            round_ += 1
            count.release()
            print(f"Node {node_id} could not decide on the leader and moves to round {round_}.")

            if count._value == n:
                print()
                for _ in range(n):
                    count.acquire()
                    barrier.release()
        mtx.release()

if __name__ == '__main__':
    mailboxes = []
    threads = []
    
    for i in range(N):
        mailboxes.append(ConSet())
        threads.append(Thread(target=nodeWork, args=[i, N]))

    for t in threads:
        t.start()

    for t in threads:
        t.join()
