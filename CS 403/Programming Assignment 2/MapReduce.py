from multiprocessing import Process
import os
from abc import ABC, abstractmethod
import zmq
import time

"""
                    X num_worker
                ->    Consumer    ->
                ->       .        ->
                ->       .        ->
                ->       .        ->
                ->    Consumer    ->
    Producer    ->    Consumer    ->    Result Collector
                ->    Consumer    ->
                ->       .        ->
                ->       .        ->
                ->       .        ->
                ->    Consumer    ->
"""

class MapReduce(ABC):
    # Constructor
    def __init__(self, num_worker:int):
        if not isinstance(num_worker, int):
            raise TypeError("num_worker must be an integer!")
        elif num_worker < 0:
            raise TypeError("num_worker cannot be non-positive integer!")
        elif num_worker > 10:
            raise TypeError("num_worker cannot be more than 10!")
        self.num_worker = num_worker

    # Partial Result
    @abstractmethod
    def Map(self, map_input):
        pass
    
    # Result
    @abstractmethod
    def Reduce(self, reduce_input):
        pass
    
    def _Producer(self, producer_input):
        context = zmq.Context()
        # pusher
        sender = context.socket(zmq.PUSH)
        sender.bind("tcp://127.0.0.1:5555")

        standart = len(producer_input) // self.num_worker
        extra = len(producer_input) % self.num_worker
        idx = 0
        for i in range(self.num_worker):
            end_idx = idx + standart
            if extra > 0:
                end_idx += 1
                extra -= 1
            sender.send_json({"payload": producer_input[idx:end_idx]})
            time.sleep(0.1)     # to avoid deadlocks
            idx = end_idx
    
    def _Consumer(self, consumer_input):
        print('Consumer PID:', os.getpid())
        
        context = zmq.Context()
        # puller
        receiver = context.socket(zmq.PULL)
        receiver.connect("tcp://127.0.0.1:5555")
        # pusher
        sender = context.socket(zmq.PUSH)
        sender.connect("tcp://127.0.0.1:5556")

        payload = receiver.recv_json()

        print(f"Map {os.getpid()} Input: {payload['payload']}")
        result = self.Map(payload["payload"])
        print(f"Map {os.getpid()} Result: {result}")

        sender.send_json(result)
    
    def _ResultCollector(self):
        print ('ResultCollector PID:', os.getpid())

        context = zmq.Context()
        # puller
        receiver = context.socket(zmq.PULL)
        receiver.bind("tcp://127.0.0.1:5556")

        partial_results = []
        for i in range(self.num_worker):
            payload = receiver.recv_json()
            partial_results.append(payload)

        print(f"Reducer Data: {partial_results}")
        reduced_results = self.Reduce(partial_results)
        print(f"Results {reduced_results}")

        with open("results.txt", 'w') as file:
            file.write(str(reduced_results))
    
    def start(self, filename):
        with open(filename, 'r') as file:
            filedata = file.read().split('\n')
        inputs = []
        for line in filedata:
            if len(line) > 0:
                linedata = line.split()
                linedata_int = [int(s) for s in linedata]
                inputs.append(linedata_int)
        print(f"Initial Data: {inputs}")

        Producer = Process(target=self._Producer, args=(inputs,))
        Consumers = []
        for i in range(self.num_worker):
            Consumer = Process(target=self._Consumer, args=(i,))
            Consumers.append(Consumer)
        ResultCollector = Process(target=self._ResultCollector)

        Producer.start()
        for i in range(self.num_worker):
            Consumers[i].start()
        ResultCollector.start()

        Producer.join()
        for i in range(self.num_worker):
            Consumers[i].join()
        ResultCollector.join()
