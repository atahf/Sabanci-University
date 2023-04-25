#include <iostream>
#include <string>

using namespace std;

string keyGen(string key, string plainText) {
	int t = floor((double)plainText.length()/key.length());
	string res = "";
	for(int i = 0; i <= t; i++) 
		res += key;
	return res.substr(0, plainText.length());
}

unsigned char permute(unsigned char c) {
	unsigned char res = 0;
								// 7 6 5 4   3 2 1 0  =>  6 4 7 5   2 0 3 1
	res |=  (c << 2 & 0x4);		//0th to 2nd index
	res |=  (c << 1 & 0x8);		//2nd to 3th index
	res |=  (c >> 1 & 0x1);		//1st to 0th index
	res |=  (c >> 2 & 0x2);		//3rd to 1st index
	res |=  (c << 2 & 0x40);	//4th to 6th index
	res |=  (c << 1 & 0x80);	//6th to 7th index
	res |=  (c >> 1 & 0x10);	//5th to 4th index
	res |=  (c >> 2 & 0x20);	//7th to 5th index

	return res;
}

void cypher(string key, string plainText) {
	string newKey = keyGen(key, plainText);
	cout << "Ciphertext: ";
	for(int i = 0; i < plainText.length(); i++)
		cout << hex << (int)(permute(plainText[i]) ^ (unsigned char)newKey[i]);
	cout << endl;
}

int main() {
	string key, plainText;
	cout << "*** Welcome to the Simple Data Encryption Application ***\nPlease enter the encryption key: ";
	cin >> key;
	getline(cin, plainText);
	while(true) {
		cout << "Please enter the plaintext to be encrypted: ";
		getline(cin, plainText);
		if(cin.eof())
			break;
		cypher(key, plainText);
		cout << endl;
	}
	return 0;
}
//Ataollah Hosseinzadeh Fard	ID: 28610