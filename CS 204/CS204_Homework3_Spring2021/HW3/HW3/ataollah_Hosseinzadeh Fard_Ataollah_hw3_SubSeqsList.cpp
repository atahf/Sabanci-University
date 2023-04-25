#include "ataollah_Hosseinzadeh Fard_Ataollah_hw3_SubSeqsList.h"

//Constructor of class
SubSeqsList::SubSeqsList() {
	hHead = NULL;
}

//destructor of class
SubSeqsList::~SubSeqsList() {
	SubSeqHeadNode *ptr;
	while (hHead != NULL) {
		ptr = hHead;
		hHead = hHead->next;
		SubSeqNode *tmpNode = ptr->sHead, *tmp2Node;
		delete ptr;
		while(tmpNode != NULL) {
			tmp2Node = tmpNode;
			tmpNode = tmpNode->next;
			delete tmp2Node;
		}
	}
}

/*a private member function that with given number makes all 
  possible ascending combinations and adds to the class object.*/
void SubSeqsList::gen_seqs(const int &num) {
	if(hHead != NULL) {
		SubSeqHeadNode *ptr = hHead, *toBeAdded = NULL, *addTMP = toBeAdded;
		while(ptr != NULL) {
			SubSeqHeadNode *tmp = new SubSeqHeadNode((ptr->size) + 1, clone(ptr, num));
			SubSeqNode *sHead = ptr->sHead;
			while(sHead->next != NULL) {
				sHead = sHead->next;
			}
			if(sHead->value < num && sHead->next == NULL) {
				if (toBeAdded == NULL) {
					addTMP = tmp;
					toBeAdded = tmp;
				}
				else {
					addTMP->next = tmp;
					addTMP = addTMP->next;
				}
			}
			ptr = ptr->next;
		}
		insert(toBeAdded);
	}
}

//a private member function that clones (copies) the given SubSeqNode and adds the given number to the end of it.
SubSeqNode * SubSeqsList::clone(SubSeqHeadNode *&Hnode, const int &num) const {
	if(Hnode == NULL)
		return NULL;
	SubSeqNode *headClone = new SubSeqNode(Hnode->sHead->value);
	SubSeqNode *ptr = Hnode->sHead->next;
	SubSeqNode*ptrClone = headClone;
	while(ptr != NULL) {
		ptrClone->next = new SubSeqNode(ptr->value);
		ptr = ptr->next;
		ptrClone = ptrClone->next;
	}
	ptrClone->next = new SubSeqNode(num);
	return headClone;
}

/*a private member function that takes a SubSeqHeadNode that contains all possible combination 
  to be added and via loop adds each SubSeqHeadNode inside of it to a proper place in hHead.*/
void SubSeqsList::insert(SubSeqHeadNode *combinations) {
	if(hHead != NULL) {
		SubSeqHeadNode *ptr, *node = combinations;
		while(node != NULL){
			ptr = hHead;
			while(ptr->next != NULL && ptr->next->size < node->size) {
				ptr = ptr->next;
			}
			bool _continue = true;
			while(ptr->next != NULL && ptr->next->size == node->size && _continue) {
				SubSeqNode *tmpPtr = ptr->next->sHead, *tmpNode = node->sHead;
				while(tmpPtr != NULL && _continue) {
					if(tmpPtr->value >tmpNode->value) {
						_continue = false;
					}
					else {
						tmpPtr = tmpPtr->next;
						tmpNode = tmpNode->next;
					}
				}
				if(_continue) {
					ptr = ptr->next;
				}
			}
			SubSeqHeadNode *tmp = new SubSeqHeadNode(node->size, node->sHead);
			tmp->next = ptr->next;
			ptr->next = tmp;
			node = node->next;
		}
	}
}

//this fuinctions goes over single sized nodes and searches for existence of given number.
bool SubSeqsList::search(const int &num) const {
	SubSeqHeadNode * tmp = hHead;
	while (tmp != NULL) {
		if(tmp->size == 1 && tmp->sHead != NULL && tmp->sHead->value == num)
			return true;
		tmp = tmp->next;
	}
	return false;
}

/*this function adds the given number as a single sized SubSeqHeadNode to hHead but before doing it, first
  calls gen_seqs(const int &num) and adds all possible combinations before adding the number itself.*/
void SubSeqsList::add(const int &num) {
	if (search(num)) {
		cout << num << " is already in the list!" << endl;
	}
	else {
		gen_seqs(num);
		if(hHead == NULL) {
			hHead = new SubSeqHeadNode(1, num);
		}
		else {
			SubSeqHeadNode *p = hHead;
			while(p->next != NULL && p->next->size <= 1) {
				p = p->next;
			}
			SubSeqHeadNode *tmp = new SubSeqHeadNode(1, num);
			tmp->next = p->next;
			p->next = tmp;
		}
		cout << "Subsequence(s) containing " << num << " has/have been added" << endl;
	}
}

//deletes all the SubSeqHeadNode or SubSeqHeadNodes those have the number within them.
void SubSeqsList::delete_one(const int &num) {
	if(!search(num)) {
		cout << "There is no subsequence that contains " << num << " to be deleted" << endl;
	}
	else {
		SubSeqHeadNode *ptr = hHead;
		while (ptr != NULL) {
			SubSeqNode *tmpExist = ptr->sHead;
			bool exists = false;
			while (tmpExist != NULL && !exists) {
				if (tmpExist->value == num)
					exists = true;
				tmpExist = tmpExist->next;
			}
			if(exists) {
				SubSeqHeadNode *tmp = ptr;
				ptr = ptr->next;
				if(tmp == hHead) {
					hHead = hHead->next;
					delete tmp;
				}
				else {
					SubSeqHeadNode *head = hHead;
					while(head->next != tmp) {
						head = head->next;
					}
					head->next = tmp->next;
					SubSeqNode *tmpNode = tmp->sHead, *tmp2Node;
					delete tmp;
					while(tmpNode != NULL) {
						tmp2Node = tmpNode;
						tmpNode = tmpNode->next;
						delete tmp2Node;
					}
				}
			}
			else {
				ptr = ptr->next;
			}
		}
		cout << "All subsequence(s) containing " << num << " has/have been deleted" << endl;
	}
}

//displays all the elements of the class object it proper style.
void SubSeqsList::display() const {
	cout << "\nFINAL CONTENT" << endl;
	if (hHead == NULL) {
		cout << "List is empty!" << endl;
	}
	else {
		SubSeqHeadNode *ptr = hHead;
		while (ptr != NULL) {
			cout << ptr->size << " |";
			while (ptr->sHead != NULL) {
				cout << " " << ptr->sHead->value;
				if (ptr->sHead->next != NULL) {
					cout << " ->";
				}
				ptr->sHead = ptr->sHead->next;
			}
			cout << endl;
			ptr = ptr->next;
		}
		cout << endl;
	}
}

//Ataollah Hosseinzadeh Fard
//ID: 28610