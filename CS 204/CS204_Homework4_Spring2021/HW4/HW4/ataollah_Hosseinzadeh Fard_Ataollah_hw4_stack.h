#ifndef STACK_H
#define STACK_H

#include <iostream>

using namespace std;

struct Position {
	int row, column;
	Position(int _row = 0, int _column = 0) : row(_row), column(_column) {};		//constructor for the Position struct
};

struct Node {
	Position position;
	Node *next;
	Node(Position _position = Position(), Node* _next = NULL) :position(_position), next(_next) {};		//constructor for the Node struct
};

class Stack {
	public:
		Stack();					//the defult constructor
		~Stack();					//the defult destructor
		void push(int &, int &);	//inserts a node to the top
		void pop(int &, int &);		//deletes the top element
		bool isEmpty();				//check whether it is empty or not
	private:
		Node *top;
};
#endif

//Ataollah Hosseinzadeh Fard
//ID: 28610