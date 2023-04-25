/*
	Codes written by Ataollah Hosseinzadeh Fard,
	with ID 28610 for the CS300 HW2.

	This is a custom templated open addressing
	HashTable class, which most parts of it is
	taken from CS300 lecture codes.

	To solve som compability issues the Hash
	function is written seperately for string
	and int (and it would be better to write for
	all other data types which are possible to
	use for this class but due to being custom
	for only CS300 HW2 only type fully supported
	is string and int Hash() is just a figure), and
	for comparison between string and const string
	== and != operators are overloaded.

	Note: while using these class header and cpp
	      move both files to project directory
		  but since class is temlated, in compiler
		  cpp file should be excluded from project.
*/

#include "HashTable.h"

int Hash(const dict<string> & keyPair, int tableSize) {
	int hashedInt = 0;
	for(int i = 0; i < keyPair.data.length(); i++) {
		unsigned char tmp = keyPair.data.at(i);
		hashedInt += tmp;
	}
	return hashedInt % tableSize;
};

int Hash(const string & key, int tableSize) {
	return ((unsigned char)key.at(0) % tableSize);
};

int Hash(int key, int tableSize) {
	return 0;
};

template<class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj & notFound, int size): ITEM_NOT_FOUND(notFound), array(nextPrime(size)) {
	currentSize = 0;
	makeEmpty();
};

template<class HashedObj>
int HashTable<HashedObj>::getIndex(const HashedObj & x) const {
	int currentPos = findPos(x);
	if(isActive(currentPos))
		return currentPos;
	return -1;
};

template<class HashedObj>
HashedObj HashTable<HashedObj>::getElement(int idx) const {
	return array[idx].element;
}

template<class HashedObj>
int HashTable<HashedObj>::getCurrentSize() const {
	return currentSize;
};

template<class HashedObj>
const HashedObj HashTable<HashedObj>::get(int idx) const {
	if(isActive(idx))
		return array[idx].element;
	return ITEM_NOT_FOUND;
};

template<class HashedObj>
const HashTable<HashedObj> & HashTable<HashedObj>::operator=(const HashTable<HashedObj> & rhs) {
	makeEmpty();

	this.currentSize = rhs.currentSize();
	this.ITEM_NOT_FOUND = rhs.ITEM_NOT_FOUND;
	this.array = rhs.array;

	return this;
};

template<class HashedObj>
int HashTable<HashedObj>::findPos(const HashedObj & x) const {
	int p = Hash(x, array.size());

	HashedObj tmp = array[p].element;
	while(array[p].info != EMPTY && array[p].element != x) {
		p++;
		if(p >= array.size())
			p = 0;
	}

	return p;
};

template<class HashedObj>
bool HashTable<HashedObj>::isActive(int currentPos) const {
	return array[currentPos].info == ACTIVE;
};

template<class HashedObj>
void HashTable<HashedObj>::remove(const HashedObj & x) {
	int currentPos = find(x);

	if(isActive(currentPos))
		array[currentPos].info = DELETED;
};

template<class HashedObj>
const HashedObj & HashTable<HashedObj>::find(const HashedObj & x) const {
	int currentPos = findPos(x);

	if(isActive(currentPos))
		return array[currentPos].element;
	return ITEM_NOT_FOUND;
};

template<class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj & x) {
	int currentPos = findPos(x);

	if(isActive(currentPos))
		return;

	array[currentPos] = HashEntry(x, ACTIVE);

	if(++currentSize >= array.size())
		rehash();
};

template<class HashedObj>
void HashTable<HashedObj>::rehash() {
	vector<HashEntry> oldArray = array;

	array.resize(nextPrime(2 * oldArray.size()));

	for(int j = 0; j < array.size(); j++)
		array[j].info = EMPTY;
	
	currentSize = 0;
	for(int i = 0; i < oldArray.size(); i++)
		if(oldArray[i].info == ACTIVE)
			insert(oldArray[i].element);
};

template<class HashedObj>
void HashTable<HashedObj>::makeEmpty() {
	for(int i = 0; i < array.size(); i++)
		array[i] = HashEntry();
};

template<class HashedObj>
bool HashTable<HashedObj>::operator==(const HashTable<HashedObj> & rhs) const {
	return (this.currentSize == rhs.currentSize && this.ITEM_NOT_FOUND == rhs.ITEM_NOT_FOUND && this.array == rhs.array);
};

template<class HashedObj>
bool HashTable<HashedObj>::operator!=(const HashTable<HashedObj> & rhs) const {
	return (this.currentSize =! rhs.currentSize || this.ITEM_NOT_FOUND =! rhs.ITEM_NOT_FOUND || this.array =! rhs.array);
};

template<class HashedObj>
bool operator==(const dict<HashedObj> & lhs, const dict<HashedObj> & rhs) {
	return (lhs.data == rhs.data);
}

template<class HashedObj>
bool operator!=(const dict<HashedObj> & lhs, const dict<HashedObj> & rhs) {
	return (lhs.data != rhs.data);
}

bool isPrime(int n) {
	if(n == 2 || n == 3)
		return true;

	if(n == 1 || n % 2 == 0)
		return false;

	for(int i = 3; i * i <= n; i += 2)
		if(n % i == 0)
			return false;

	return true;
};

int nextPrime(int n)  {
	if(n % 2 == 0)
		n++;

	for(; !isPrime(n); n += 2);

	return n;
};