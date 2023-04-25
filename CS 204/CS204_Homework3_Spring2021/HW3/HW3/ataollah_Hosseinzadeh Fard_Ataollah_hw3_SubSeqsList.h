#ifndef SubSeqsList_h
#define SubSeqsList_h

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct SubSeqNode {
	int value;									//the value to be strored in a single element of list
	SubSeqNode * next;							//pointer to reach next element
	SubSeqNode(): value(0), next(NULL){}		//default constructor that creates an empty linked lsit
	SubSeqNode(int n): value(n), next(NULL){}	//constructor that creates single elemnt with given number as value
};

struct SubSeqHeadNode {
	int size;																		// number of elements in the subsequence
	SubSeqNode * sHead;																// head of the subsequence list
	SubSeqHeadNode * next;															//pointer to reach next element
	SubSeqHeadNode(): size(0), sHead(NULL), next(NULL){}							//default constructor that creates an empty 2D linked list
	SubSeqHeadNode(int s, SubSeqNode *node): size(s), sHead(node), next(NULL){}		//constructor that creates a 2D linked list with given 1D linked list as sub elemnt with size given
	SubSeqHeadNode(int s, int n): size(s), sHead(new SubSeqNode(n)), next(NULL){}	//constructor that creates single elemnted 2D linked list with given number as the onle value with size given
};

class SubSeqsList {
	public:
		SubSeqsList();						//default constructor that creates an empty list
		~SubSeqsList();						//default destructor that deletes all list
		bool search(const int &num) const;	//checks whether the num is exits in object or not
		void add(const int &num);			//add the num to object
		void delete_one(const int &num);	//deletes only the given num from object
		void display() const;				//displays all content of the 2D linked list (class object)
	private:
		SubSeqHeadNode * hHead;

		//following functions are placed in private so they are meant to be used only in class's code not in 3rd party code such as main cpp file
		void gen_seqs(const int &num);										//generates all possible sub sequences
		SubSeqNode * clone(SubSeqHeadNode *&Hnode, const int &num) const;	//copies a SubSeqNode struct object
		void insert(SubSeqHeadNode *combinations);							//inserts a SubSeqHeadNode struct object to the best position of the 2D list in class object
};

#endif

//Ataollah Hosseinzadeh Fard
//ID: 28610