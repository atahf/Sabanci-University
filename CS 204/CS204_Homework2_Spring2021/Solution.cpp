#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct Node { //node structure
	int num;
	Node* next;

	Node(): num(0), next(NULL){}
	Node(int num_, Node* next_ = NULL): num(num_), next(next_){}//note that, constructor has a default parameter
};

void print(Node* head)//Function that prints the linked list
{
	while (head != NULL) {
		cout << head->num << " ";
		head = head->next;
	}
	cout << "\n\n";
}


bool find(Node* head, int num) // Function that returns true if the given value is already in the list, otherwise returns false
{
	while (head != NULL) {
		if (head->num == num)
			return true;
		else
			head = head->next;
	}
	return false;
}


void deleteAfter(Node* & ptr) //function that deletes the the given node and following nodes
{
	Node* tmp = NULL;
	if (ptr == NULL)
	{
		cout << "None";
	}
	while (ptr != NULL) {
		tmp = ptr->next;
		cout << ptr->num << " ";
		delete ptr;
		ptr = tmp;
	}
	cout << endl;
}

void insertNode(Node* & head, int num, string order) //insert function, function that inserts a new node with the given value
{
	Node* tmp = head;

	if (order == "A") { //if ordering mode is ascending

		if (head == NULL || num < head->num) { //if the linked list is empty or the given value is smaller than the head, we need to update our head
			head = new Node(num, head);//our new head will point our old head, first node is created then it is assigned to our head
			cout << "Deleted nodes: ";
			deleteAfter(head->next);
			cout << "Appended: " << num << endl;
			return;
		}

		while (tmp->next != NULL && tmp->next->num < num) {//if the given value will not update the head, so we need to find appropriate position for it
			tmp = tmp->next;
		}
	}
	else {//if ordering mode is descending
		if (head == NULL || num > head->num) {//if the linked list is empty or the given value is greater than the head, we need to update our head
			head = new Node(num, head);
			cout << "Deleted nodes: ";
			deleteAfter(head->next);
			cout << "Appended: " << num << endl;
			return;
		}

		while (tmp->next != NULL && tmp->next->num > num) {//if the given value will not update the head, so we need to find appropriate position for it
			tmp = tmp->next;
		}
	}

	Node* latter = tmp->next; //after finding the appropriate position, create the new node and insert
	cout << "Deleted nodes: ";
	deleteAfter(latter);
	tmp->next = new Node(num);
	cout << "Appended: " << num << endl;

}

bool checkInput(string& order)//input check function
{
	if (order.length() != 1)
		return false;
	if (order != "A" && order != "D")
		return false;

	return true;
}

int main() 
{
	string numbers = "";//ex. 1 5 4 8 6 4
	string order = "";//ex. "A"
	Node* head = NULL;//head of our linked list, initially empty

	cout << "Please enter the order mode (A/D): ";
	cin >> order;
	
	
	while (!checkInput(order)) { //input check for the ordering mode, if input check fails, asks for a new input untill correct input is given.
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		order = "";
		cout << "Please enter the order mode again (A/D): ";
		cin >> order;
	}

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "Please enter the numbers in a line: ";
	getline(cin, numbers);//reading the entire line as a input
	cout << endl;
	int tmp;
	stringstream ss(numbers);//stringstream for splitting line into integers
	
	while (ss >> tmp) {
		cout << "Next number: " << tmp << endl;
		if (find(head, tmp)) { //if find function returns true, displaying appropriate message
			cout << tmp << " is already in the list!\n";
		}
		else { //else insert the new value
			insertNode(head, tmp, order);
		}
		cout << "List content: ";
		print(head);
	}
	
	
	if(head == NULL){
		cout << "The list is empty at the end of the program and nothing is deleted\n";
	}
	else{
		cout << "All the nodes are deleted at the end of the program: ";
		deleteAfter(head);//since deleteAfter function deletes the given node and all the nodes after it, giving head as a parameter means deleting the all list.
	}
	return 0;
}	