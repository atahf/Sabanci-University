#ifndef BOARD_H
#define BOARD_H

template <class T>
struct Block{
	T value;
	bool open;

	Block() {};										//default constructor
	Block(T _val): value(_val), open(false) {};		//parameter constructor
};

template <class T>
class Board{
	public:
		Board(int, int);						//parameter constructor
		~Board();								//destructor

		void readBoardFromFile(ifstream &);		//set values on board
		void displayBoard() const;				//display board
		void closeCard(int, int);				//closes a card by index
		int getRow() const;						//returns row number
		int getColumn() const;					//returns column number

		Block<T> getItem(int, int) const;		//accessor
		bool isOpen(int, int) const;			//accessor

		void openCard(int, int);				//mutator
		
	private:
		Block<T> **matrix;
		int Row, Column;
};

#include "ataollah_Hosseinzadeh Fard_Ataollah_hw6_Board.cpp"

#endif

//Ataollah Hosseinzadeh Fard
//ID: 28610