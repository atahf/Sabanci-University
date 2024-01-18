import sys
from multiprocessing import Process
import random
import zmq

IP = "127.0.0.1"
PORT = 5577
context = zmq.Context()

def PaxosNode(ID, prob, N, val, numRounds):
    def send(msg, sender, destination):
        pushes[destination].send_json({'msg': msg, 'id': sender})

    def sendFailure(msg, proposer, i, prob):
        crash_status = random.choices([True, False], weights=(prob, 1-prob), k=1)[0]

        if crash_status:
            send(f"CRASH {proposer}", proposer, i)
        else:
            send(msg, proposer, i)

    def broadcastFailure(msg, proposer, N, prob):
        for i in range(N):
            sendFailure(msg, proposer, i, prob)
    
    pull = context.socket(zmq.PULL)
    pull.bind(f"tcp://{IP}:{PORT+ID}")

    pushes = []
    for i in range(N):
        push = context.socket(zmq.PUSH)
        push.connect(f"tcp://{IP}:{PORT+i}")
        pushes.append(push)

    maxVotedRound = -1
    maxVotedVal = None
    proposeVal = None
    decision = None

    for currRound in range(numRounds):
        if currRound % N == ID:
            print(f"ROUND {currRound} STARTED WITH INITIAL VALUE {val}")
            broadcastFailure("START", ID, N, prob)
            tmp_maxVotedRound = -1
            tmp_maxVotedVal = -1

            join_count = 0
            seenJoin = set()
            while len(seenJoin) != N:
                tmp = pull.recv_json()
                if not tmp["id"] in seenJoin:
                    seenJoin.add(tmp["id"])
                    newMsg = tmp["msg"]
                    print(f"LEADER OF {currRound} RECEIVED IN JOIN PHASE: {newMsg}")
                    if not "CRASH" in newMsg:
                        join_count += 1
                        if "JOIN" in newMsg:
                            parsedMsg = newMsg.split()
                            acc_maxVotedRound = int(parsedMsg[1])
                            if acc_maxVotedRound > -1:
                                acc_maxVotedVal = int(parsedMsg[2])
                                if tmp_maxVotedRound < acc_maxVotedRound:
                                    tmp_maxVotedRound = acc_maxVotedRound
                                    tmp_maxVotedVal = acc_maxVotedVal
                        else:
                            if tmp_maxVotedRound < maxVotedRound:
                                tmp_maxVotedRound = maxVotedRound
                                tmp_maxVotedVal = maxVotedVal
            
            if join_count <= N / 2:
                print(f"LEADER OF ROUND {currRound} CHANGED ROUND")
                for i in range(N):
                    if i != ID:
                        send("ROUNDCHANGE", ID, i)
            else:
                maxVotedRound = tmp_maxVotedRound
                maxVotedVal = tmp_maxVotedVal
                if maxVotedRound == -1:
                    maxVotedVal = val
                proposeVal = maxVotedVal
                print(f"LEADER OF ROUND {currRound} PROPOSES {proposeVal}")
                broadcastFailure(f"PROPOSE {proposeVal}", ID, N, prob)

                vote_count = 0
                seenVotes = set()
                while len(seenVotes) != N:
                    tmp = pull.recv_json()
                    if not tmp["id"] in seenVotes:
                        seenVotes.add(tmp["id"])
                        newMsg = tmp["msg"]
                        print(f"LEADER OF {currRound} RECEIVED IN JOIN VOTE: {newMsg}")
                        if "PROPOSE" in newMsg or "VOTE" in newMsg:
                            vote_count += 1
                
                if vote_count > N / 2:
                    decision = proposeVal
                    maxVotedRound = currRound
                    maxVotedVal = proposeVal
                    print(f"LEADER OF {currRound} DECIDED ON VALUE {decision}")
        else:
            while True:
                startMsg = pull.recv_json()['msg']
                if len(startMsg) > 0:
                    print(f"ACCEPTOR {ID} RECEIVED IN VOTE JOIN: {startMsg}")

                    if "CRASH" in startMsg:
                        send(f"CRASH {currRound % N}", ID, currRound % N)
                    else:
                        sendFailure(f"JOIN {maxVotedRound} {maxVotedVal}", ID, currRound % N, prob)
                    
                    while True:
                        proposeMsg = pull.recv_json()["msg"]
                        if len(proposeMsg) > 0:
                            print(f"ACCEPTOR {ID} RECEIVED IN VOTE PHASE: {proposeMsg}")

                            if "PROPOSE" in proposeMsg:
                                maxVotedRound = currRound
                                maxVotedVal = int(proposeMsg.split()[1])
                                sendFailure("VOTE", ID, currRound % N, prob)
                            elif "CRASH" in proposeMsg:
                                send(f"CRASH {currRound % N}", ID, currRound % N)
                            break
                    break

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python paxos.py [NUM_NODES] [CRASH_PROB] [NUM_ROUNDS]")
        sys.exit(1)
    
    numProc = int(sys.argv[1])
    prob = float(sys.argv[2])
    numRounds = int(sys.argv[3])
    print(f"NUM NODES: {numProc}, CRASH PROB: {prob}, NUM ROUNDS: {numRounds}")

    nodes = []
    for i in range(numProc):
        initVal = random.choice([0, 1])
        newNode = Process(target=PaxosNode, args=(i, prob, numProc, initVal, numRounds,))
        newNode.start()
        nodes.append(newNode)

    for i in range(numProc):
        nodes[i].join()


