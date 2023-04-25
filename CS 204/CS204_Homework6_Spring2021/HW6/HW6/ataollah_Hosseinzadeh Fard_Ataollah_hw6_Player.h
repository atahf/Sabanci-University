#ifndef PLAYER_H
#define PLAYER_H

template<class T>
class Player{
	public:
		Player(Board<T> &_board): score(0), board(_board) { };	//parameter constructor that links a board obj to class obj

		T openCard(int, int);									//opens a card on board
		bool validMove(int, int) const;							//checks the validity of choice
		void increaseNumberOfSuccess();							//increament of score
		int getNumberOfSuccess() const;							//returns score

	private:
		Board<T> & board;										//linekd board
		int score;
};

#include "ataollah_Hosseinzadeh Fard_Ataollah_hw6_Player.cpp"

#endif

//Ataollah Hosseinzadeh Fard
//ID: 28610