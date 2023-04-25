#include <iostream>
#include "ataollah_Hosseinzadeh Fard_Ataollah_hw3_SubSeqsList.h"

using namespace std;

/*basically program create a SubSeqsList class object in name of S and then gets
  a line of input from user, if it is empty, it will directly try to display 
  content of the object, otherwise it will try to add number, but if number is 
  negative, it will try to delete that numbers's positive form from class object.*/

int main() {
	string line;
	SubSeqsList S;
	cout << "Please enter the numbers in a line: ";
	getline(cin, line);
	cout << endl;
	if (!line.empty()) {
		int num;
		istringstream input(line);
		while (input >> num) {
			if (num < 0) {
				S.delete_one(abs(num));
			}
			else {
				S.add(num);
			}
		}
	}
	S.display();
	return 0;
}

//Ataollah Hosseinzadeh Fard
//ID: 28610