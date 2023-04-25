#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct node{
	int value;
	node* next;
};

//this Function prints all contents of linked list.
string listContent(node *head) {
	string res = "";
	node * ptr = head;
	while (ptr != NULL) {
		res = to_string(ptr ->value) + " " + res;
		ptr = ptr->next;
	}
	return res;
}

//this function seraches for exisence of a given value in given linked list
bool SearchList(node *head, int target) {
	while (head != NULL) {
		if(head->value == target)
			return true;			
		head = head->next;
	}
	return false;
}

/*this function makes sure according to order whether A or D makes sure that given value 
  can be appended to the list. By iterating over linked list according to order checks 
  all values and if order condition satisfies returns true otherwise returns false.*/
bool eligible(node *head, int value, char order) {
	while (head != NULL) {
		if(order == 'A' && head->value > value) {
			return false;
		}
		else if(order == 'D' && head->value < value) {
			return false;
		}
		head = head->next;
	}
	return true;
}

//this function finds the nodes to be deleted and adds their values to a string and return that string.
string nodesToBeDeleted(node *head, int target, char order) {
	string res = "";
	node *ptr = head;
	while(ptr != NULL) {
		if(order == 'A' && ptr->value > target) {
			res = to_string(ptr ->value) + " " + res;
		}
		else if(order == 'D' && ptr->value < target) {
			res = to_string(ptr ->value) + " " + res;
		}
		ptr = ptr->next;
	}
	return res;
}

/*this function is mixture of "delete all nodes" and "delete a node" functions, accoring to 
  given option string function decides whether to delete all nodes or if not it decides to 
  whethr to delete which nodes accoring to given order, and this funtion is the only function 
  that linked list is being passed as refrence due to gain permission to edit list.*/
void deleteByOption(node * & head, string option, char order) {
	node *ptr;
	bool _continue = true;
	while(_continue && head != NULL) {
		if (option != "ALL" && order == 'A' && head->value < stoi(option)) {
			_continue = false;
		}
		else if (option != "ALL" && order == 'D' && head->value > stoi(option)) {
			_continue = false;
		}
		else {
			ptr = head;
			head = head->next;
			delete ptr;
		}
	}
}

/*in the main part basically, first takes input untill the order input conditions are
  satisfied, then is takes a line input consisting nubers and by a loop it iterates
  and one by one tries to put numbers to list by ckecking needed conditions.*/
int main() {
	string io, numbers;
	char order;
	cout << "Please enter the order mode (A/D): ";
	getline(cin, io);
	istringstream order_in(io);
	order_in >> order;
	while(order != 'A' && order != 'D') {
		cin.clear();
		cout << "Please enter the order mode again (A/D): ";
		getline(cin, io);
		istringstream order_in_again(io);
		order_in_again >> order;
	}
	cin.clear();
	cout << "Please enter the numbers in a line: ";
	getline(cin, numbers);
	cin.clear();
	cout << endl;
	if(numbers.empty()) {
		cout << "The list is empty at the end of the program and nothing is deleted" << endl;
	}
	else {
		istringstream numbers_in(numbers);
		int num;
		node *head = NULL, *temp = NULL;
		while(numbers_in >> num){
			cout << "Next number: " << num << endl;
			if(SearchList(head, num)){
				cout << num << " is already in the list!" << endl;
			}
			else {
				string deletedResults = "None";
				if(!eligible(head, num, order)) {
					deletedResults = nodesToBeDeleted(head, num, order);
					deleteByOption(head, to_string(num), order);
				}
				cout << "Deleted nodes: " << deletedResults << endl;
				temp = new node();
				temp->value = num;
				temp->next = head;
				head = temp;
				cout << "Appended: " << num << endl;
			}
			cout << "List content: " << listContent(head) << endl << endl;
		}
		cout << "All the nodes are deleted at the end of the program: " << listContent(head) << endl;
		deleteByOption(head, "ALL", order);
	}
	return 0;
}
//Ataollah Hosseinzadeh Fard
//ID: 28610