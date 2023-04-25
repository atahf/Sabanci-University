#ifndef WALLET_H
#define WALLET_H

#include <iostream>
#include <string>

using namespace std;

struct Money {
	string currency;
	double amount;
};

class Wallet {
	public:
		Wallet();											//default constrictor
		Wallet(const Wallet &);								//deep copy constrictor
		~Wallet();											//default desstrictor

		int size() const;									//returns the size of the Money 1D dynamic array
		Money Nth_element(const int &) const;				//returns the Nth element of the Money 1D dynamic array

		Wallet operator +(const Wallet) const;				//adds a Wallet obejct to the another Wallet object
		Wallet operator +(const Money &) const;				//adds a a single Money obejct to the Wallet object
		Wallet operator -(const Money &) const;				//reduces amoount or removes a Money object from Wallet object
		const Wallet & operator =(const Wallet &);			//if lhs is not the same with rhs, it will copy rhs to lhs
		const Wallet & operator +=(const Wallet &);			//adds a Wallet obejct to class's object

	private:
		Money *currencies;									//dynamic 1D array of money object
		int countOfCurrencies;								//number of elements inside the currencies array (size of array)
};

ostream & operator <<(ostream &, const Wallet &);			//prints elements of the class object

bool operator ==(const Wallet &w1, const Wallet &w2);		//is as same as operator
bool operator >=(const Wallet &w, const Money &m);			//all money objects of lhs are greater or eual to the rhs money
bool operator <=(const Money &m, const Wallet &w);			//rhs money is lower or equal to all the money objects in rhs

#endif

//Ataollah Hosseinzadeh Fard
//ID: 28610