#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "ataollah_Hosseinzadeh Fard_Ataollah_hw6_Board.h"

using namespace std;

//parameter constructo that creates a board in size of given parameters
template<class T>
Board<T>::Board(int _row, int _col) {
	Row = _row;
	Column = _col;
	matrix = new Block<T>*[_row];
    for (int i = 0; i < Row; i++) {
        matrix[i] = new Block<T>[_col];
    }
};

//destructor
template<class T>
Board<T>::~Board() {
	for (int i = 0; i < Row; i++) {
       delete[] matrix[i];
    }
	delete[] matrix;
	matrix = NULL;
};

//reads values in file and sets them in proper indexes in board and their status are false by defalut
template<class T>
void Board<T>::readBoardFromFile(ifstream &input) {
	T tmp;
	string line;
	for(int i = 0; i < Row; i++) {
		getline(input, line);
		istringstream in(line);
		for(int j = 0; j < Column; j++) {
			in >> tmp;
			matrix[i][j] = Block<T>(tmp);
		}
	}
};

//displays board, if status is flase instead of value will display X
template<class T>
void Board<T>::displayBoard() const {
	for(int i = 0; i < Row; i++) {
		for(int j = 0; j < Column; j++) {
			if(matrix[i][j].open){
				cout << matrix[i][j].value << " ";
			}
			else{
				cout << "X ";
			}
		}
		cout << endl;
	}
};

//closes a card by indexes
template<class T>
void Board<T>::closeCard(int _row, int _col) {
	matrix[_row][_col].open = false;
};

//return the number of rows
template<class T>
int Board<T>::getRow() const {
	return Row;
};

//return the number of columns
template<class T>
int Board<T>::getColumn() const {
	return Column;
};

//accessor
//returns a card by index
template<class T>
Block<T> Board<T>::getItem(int _row, int _col) const {
	return matrix[_row][_col];
};

//accessor
//returns whether a card is open or not
template<class T>
bool Board<T>::isOpen(int _row, int _col) const {
	return matrix[_row][_col].open;
};

//mutator
//opens a card by indexes
template<class T>
void Board<T>::openCard(int _row, int _col) {
	matrix[_row][_col].open = true;
};

//Ataollah Hosseinzadeh Fard
//ID: 28610