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

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

template<class HashedObj>
struct dict{
	HashedObj data;
	int code;

	dict () {};
	dict(HashedObj obj, int _code): data(obj), code(_code) {};
};

template<class HashedObj>
class HashTable{
	public:
		explicit HashTable(const HashedObj & notFound, int size = 4096);
		HashTable(const HashTable<HashedObj> & rhs): ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), array(rhs.array), currentSize(rhs.currentSize) {};

		const HashedObj & find(const HashedObj & x) const;
		const HashedObj get(int index) const;

		int getIndex(const HashedObj & x) const;
		HashedObj getElement(int idx) const;
		int getCurrentSize() const;

		void makeEmpty(void);
		void insert(const HashedObj & x);
		void remove(const HashedObj & x);

		const HashTable & operator=(const HashTable & rhs);
		bool operator==(const HashTable<HashedObj> & rhs) const;
		bool operator!=(const HashTable<HashedObj> & rhs) const;

		enum EntryType{ACTIVE, EMPTY, DELETED};

	private:
		struct HashEntry {
			HashedObj element;
			EntryType info;

			HashEntry(const HashedObj & e = HashedObj(), EntryType i = EMPTY): element(e), info(i) {};
		};

		vector<HashEntry> array;
		int currentSize;
		const HashedObj ITEM_NOT_FOUND;

		bool isActive(int currentPos) const;
		int findPos(const HashedObj & x) const;
		void rehash(void);
};

#include "HashTable.cpp"

#endif
