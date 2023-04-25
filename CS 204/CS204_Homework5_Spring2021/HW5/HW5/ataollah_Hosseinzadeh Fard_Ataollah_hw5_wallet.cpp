#include "ataollah_Hosseinzadeh Fard_Ataollah_hw5_wallet.h"

//defult constructor
Wallet::Wallet() {
	countOfCurrencies = 0;
	currencies = new Money[countOfCurrencies];
}

//deep copy constructor
Wallet::Wallet(const Wallet &copy) {
	this->currencies = new Money[copy.countOfCurrencies];
	for(int i = 0; i < copy.countOfCurrencies; i++) {
		this->currencies[i].amount = copy.currencies[i].amount;
		this->currencies[i].currency = copy.currencies[i].currency;
	}
	this->countOfCurrencies = copy.countOfCurrencies;
}

//defult destructor
Wallet::~Wallet() {
	delete[] currencies;
}

//function that returns the count of money objects in class object
int Wallet::size() const {
	return countOfCurrencies;
}

//returns the Nth money object from class object
Money Wallet::Nth_element(const int &N) const {
	return currencies[N];
}

//returns a wallet object addition of two current class object and input class object
Wallet Wallet::operator +(const Wallet w) const {
	Wallet result(*this);
    result += w;
    return result;
}

//returns a wallet object that adds the given money object to current class object
Wallet Wallet::operator +(const Money &w) const {
	bool _placed = false;
	Wallet res(*this);
	int size = this->countOfCurrencies;
	for(int i = 0; i < size; i++) {
		if(res.currencies[i].currency == w.currency) {
			res.currencies[i].amount += w.amount;
			_placed = true; 
		}
	}
	if(!_placed) {
		Money *tmp = new Money[size + 1];
		for(int i = 0; i <= size; i++) {
			if(i == size) {
				tmp[i].amount = w.amount;
				tmp[i].currency = w.currency;
			}
			else {
				tmp[i].amount = res.currencies[i].amount;
				tmp[i].currency = res.currencies[i].currency;
			}
		}
		delete[] res.currencies;
		res.currencies = tmp;
		res.countOfCurrencies++;
	}
	return res;
}

//returns a wallet object that the given money object if its currency type exists in class object will be reduced eith given amount
Wallet Wallet::operator -(const Money &m) const {
	Wallet res(*this);
	bool _delete = false;
	for(int i = 0; i < res.countOfCurrencies; i++) {
		if(res.currencies[i].currency == m.currency && res.currencies[i].amount >= m.amount) {
			res.currencies[i].amount -= m.amount;
			if(res.currencies[i].amount == 0) {
				res.currencies[i].currency = "";
				_delete = true;
			}
		}
	}
	if(_delete) {
		Money *tmp = new Money[res.countOfCurrencies - 1];
		int j = 0;
		for(int i = 0; i < res.countOfCurrencies; i++) {
			if(res.currencies[i].currency.length() > 0) {
				tmp[j].amount = res.currencies[i].amount;
				tmp[j].currency = res.currencies[i].currency;
				j++;
			} 
		}
		delete[] res.currencies;
		res.currencies = tmp;
		res.countOfCurrencies--;
	}
	return res;
}

//return the given wallet object as current object
const Wallet & Wallet::operator =(const Wallet &w) {
	if(&w != this) {
		delete[] this->currencies;
		this->currencies = new Money[w.countOfCurrencies];
		for(int i = 0; i < w.countOfCurrencies; i++) {
			this->currencies[i].amount = w.currencies[i].amount;
			this->currencies[i].currency = w.currencies[i].currency;
		}
		this->countOfCurrencies = w.countOfCurrencies;
	}
    return *this;
}

//returns a wallet object with addition of given wallet object to current class object
const Wallet & Wallet::operator +=(const Wallet &w) {
	for(int i = 0; i < w.countOfCurrencies; i++) {
		*this = (*this) + w.currencies[i];
	}
	return *this;
}

//prints the data inside each money objects in wallet object
ostream & operator<< (ostream &os, const Wallet &w) {
	for (int i = 0; i < w.size(); i++) {
		if (i != 0)
			os << " - ";
		os << w.Nth_element(i).currency << " " << w.Nth_element(i).amount;
	}
	return os;
}

//returns true if wallets in LHS and RHS are the same, otherwise returns false
bool operator ==(const Wallet &w1, const Wallet &w2) {
	int matches = 0;
	if (w1.size() == w2.size()) {
		for(int i = 0; i < w1.size(); i++) {
			bool found = false;
			for(int j = 0; j < w2.size() && !found; j++) {
				if(w1.Nth_element(i).currency == w2.Nth_element(j).currency && w1.Nth_element(i).amount == w2.Nth_element(j).amount) {
					matches++;
					found = true;
				}
			}
		}
	}
	if(matches == w1.size())
		return true;
	else
		return false;
}

//returns true if there is no less valued money object compared to given money object
bool operator >=(const Wallet &w, const Money &m) {
	for(int i = 0; i < w.size(); i++) {
		if(w.Nth_element(i).currency == m.currency && w.Nth_element(i).amount >= m.amount) {
			return true;
		}
	}
	return false;
}

//same as >= operator but places of RHS and LHS are changed
bool operator <=(const Money &m, const Wallet &w) {
	return w >= m;
}

//Ataollah Hosseinzadeh Fard
//ID: 28610