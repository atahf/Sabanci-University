import sys
from FindCitations import FindCitations
from FindCyclicReferences import FindCyclicReferences

if __name__ == "__main__":
    args = sys.argv
    if len(args) < 4:
        print("usage: python main.py [COUNT|CYCLE] [num_worker] [input_file_path]")
    else:
        if int(args[2]) > 10:
            print("At maximum 10 workers!")
        else:
            if args[1] == "COUNT":
                mr = FindCitations(int(args[2]))
                mr.start(args[3])
            elif args[1] == "CYCLE":
                mr = FindCyclicReferences(int(args[2]))
                mr.start(args[3])
            else:
                print("type only can be either COUNT or CYCLE!")
