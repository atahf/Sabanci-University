#include "ataollah_Hosseinzadeh Fard_Ataollah_hw4_stack.h"

//the defult constructor
Stack::Stack() {
	top = NULL;
}

//the defult destructor
Stack::~Stack() {
	Node *ptr;
	while(top != NULL) {
		ptr = top;
		top = top->next;
		delete ptr;
	}
}

//inserts the given row and column to the top.
void Stack::push(int &row, int &column) {
	Node *newNode = new Node(Position(row, column));
	if (isEmpty())
		top = newNode;
	else {
		newNode->next = top;
		top = newNode;
	}
}

/*deletes the top elements and meanwhile the user can get the values 
  of top element by passing 2 temp int variable to pop function.*/
void Stack::pop (int &row, int &column) {
	if (!isEmpty()) {
		Node *temp;
		row = top->position.row;
		column = top->position.column;
		temp = top->next;
		delete top;
		top = temp;
	}
}

//checks whether the top is empty or not.
bool Stack::isEmpty() {
	if(top != NULL)
		return false;
	return true;	
}

//Ataollah Hosseinzadeh Fard
//ID: 28610