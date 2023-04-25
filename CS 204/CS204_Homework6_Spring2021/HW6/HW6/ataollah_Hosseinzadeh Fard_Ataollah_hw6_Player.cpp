#include "ataollah_Hosseinzadeh Fard_Ataollah_hw6_Board.h"

#include "ataollah_Hosseinzadeh Fard_Ataollah_hw6_Player.h"

//opens card by indexes given by using accessors and mutators of Board class
template<class T>
T Player<T>::openCard(int row, int col){
	T res = board.getItem(row, col).value;
	board.openCard(row, col);
	return res;
}

//checks whether the given indexes is a valid choice or not by using accessors of Board class
template<class T>
bool Player<T>::validMove(int row, int col) const {
	if(row < board.getRow() && col < board.getColumn() && !board.isOpen(row, col)) {
		return true;
	}
	else{
		return false;
	}
}

//increases score by 1
template<class T>
void Player<T>::increaseNumberOfSuccess() {
	score++;
}

//returns the score
template<class T>
int Player<T>::getNumberOfSuccess() const {
	return score;
}

//Ataollah Hosseinzadeh Fard
//ID: 28610