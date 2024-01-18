from MapReduce import MapReduce

class FindCitations(MapReduce):
    def Map(self, map_input):
        incoming_edges = {}
        
        for edge in map_input:
            node_a, node_b = map(str, edge)
            incoming_edges[node_b] = incoming_edges.get(node_b, 0) + 1
        
        return incoming_edges
    
    def Reduce(self, reduce_input):
        reduce_output = reduce_input[0]
        if len(reduce_input) > 1:
            for i in range(1, len(reduce_input)):
                for k, v in reduce_input[i].items():
                    reduce_output[k] = reduce_output.get(k, 0) + v
        return reduce_output
