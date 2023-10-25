#include <iostream>
#include <mutex>

using namespace std;

struct Node{
	int id;
	int size;
	int index;
	Node* next;
	
	Node(int _id, int _size, int _index, Node* _next): id(_id), size(_size), index(_index), next(_next) {}
};

class HeapManager{
	public:
		int initHeap(int);
		int myMalloc(int, int);
		int myFree(int, int);
		void print(void);
	private:
		Node* LinkedList;
		mutex mtx;
};

int HeapManager::initHeap(int size) {
	//cout << "Memory initialised\n";

	LinkedList = new Node(-1, size, 0, NULL);

	this->print();
	return 1;
}

int HeapManager::myMalloc(int id, int size) {
	this->mtx.lock();

	Node* ptr = this->LinkedList;
	Node* prev = NULL;
	while(ptr != NULL) {
		if(ptr->id == -1 && ptr->size > size) {
			cout << "Allocated for thread " << id << endl;

			int idx = ptr->index;
			ptr->size -= size;
			ptr->index += size;
			Node* newNode = new Node(id, size, idx, ptr);

			if(prev == NULL) {
				this->LinkedList = newNode;
			}
			else {
				prev->next = newNode;
			}

			this->print();
			this->mtx.unlock();
			return idx;
		}
		else if(ptr->id == -1 && ptr->size == size) {
			cout << "Allocated for thread " << id << endl;

			int idx = ptr->index;
			Node* newNode = new Node(id, size, idx, ptr->next);

			Node* toBeDeleted = ptr;
			if(prev == NULL) {
				this->LinkedList = newNode;
			}
			else {
				prev->next = newNode;
			}
			delete toBeDeleted;
			
			this->print();
			this->mtx.unlock();
			return idx;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	
	cout << "Cannot allocate, requested size " << size << " for thread " << id << " is bigger than remaining size" << endl;

	this->print();
	this->mtx.unlock();
	return -1;
}

int HeapManager::myFree(int id, int index) {
	this->mtx.lock();

	Node* ptr = this->LinkedList;
	Node* prev = NULL;
	while(ptr != NULL) {
		if(ptr-> id == id && ptr->index == index) {
			cout << "Freed for thread " << id << endl;

			ptr->id = -1;
			
			if(ptr->next != NULL && ptr->next->id == -1) {
				ptr->size += ptr->next->size;
				Node* toBeDeleted = ptr->next;
				ptr->next = ptr->next->next;
				delete toBeDeleted;
			}
			
			if(prev != NULL && prev->id == -1) {
				prev->size += ptr->size;
				Node* toBeDeleted = ptr;
				prev->next = ptr->next;
				delete toBeDeleted;
			}

			this->print();
			this->mtx.unlock();
			return 1;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	
	cout << "Cannot deallocate, there is no thread " << id << " at index of " << index << endl;

	this->print();
	this->mtx.unlock();
	return -1;
}

void HeapManager::print() {
	Node* ptr = this->LinkedList;
	while(ptr != NULL) {
		cout << "[" << to_string(ptr->id) << "][" + to_string(ptr->size) << "][" <<  to_string(ptr->index) << "]";
		if(ptr->next != NULL)
			cout << "---";
		ptr = ptr->next;
	}
	cout << endl;
}
