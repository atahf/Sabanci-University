from MapReduce import MapReduce

class FindCyclicReferences(MapReduce):
    def Map(self, map_input):
        cycle_status = {}
        seen_edges = set()

        for edge in map_input:
            node_a, node_b = edge
            tup = str((node_a, node_b))
            inv_tup = str((node_b, node_a))
            if inv_tup in seen_edges:
                cycle_status[inv_tup] = 1
            else:
                seen_edges.add(tup)
                cycle_status[tup] = 0

        return cycle_status
    
    def Reduce(self, reduce_input):
        reduce_output = {}
        seen_edges = set()

        for partial in reduce_input:
            for k, v in partial.items():
                if v == 1:
                    reduce_output[k] = 1
                else:
                    k_tup = eval(k)
                    k_inv = str((k_tup[1], k_tup[0]))
                    if k_inv in seen_edges:
                        reduce_output[k] = 1
                    else:
                        seen_edges.add(k)

        return reduce_output
