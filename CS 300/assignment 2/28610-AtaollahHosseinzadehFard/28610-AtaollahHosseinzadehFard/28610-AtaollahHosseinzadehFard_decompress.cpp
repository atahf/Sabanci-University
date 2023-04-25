/*
	Codes written by Ataollah Hosseinzadeh Fard,
	with ID 28610 for the CS300 HW2.

	In this hw a custom templated open addressing
	HashTable is used and in main cpp file the
	string ype of HashTable is used.
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include "HashTable.h"

using namespace std;

string decompress(const string s) {
	string res = "", tmp, response, prevResponse = "-1", newKey;
	int idx;
	HashTable<string> T("-1", 4096);
	for(int i = 1; i < 256; i++){
		tmp = (char) i;
		T.insert(tmp);
	}
	istringstream it(s);
	while(it >> tmp) {
		idx = stoi(tmp);
		response = T.get(idx);
		if(response != "-1") {
			res += response;
			if(prevResponse != "-1") {
				newKey = prevResponse + response.substr(0, 1);
				if(T.getCurrentSize() < 4096)
					T.insert(newKey);
			}
			prevResponse = response;
		}
		else {
			newKey = prevResponse + prevResponse.substr(0, 1);
			res += newKey;
			prevResponse = newKey;
			if(T.getCurrentSize() < 4096)
				T.insert(newKey);
		}
	}
	return res;
};

string read(string fileName) {
	string file = "";
	ifstream inFile;
	inFile.open(fileName.c_str());
	char tmpC;
	while(inFile.get(tmpC))
		file += tmpC;
	inFile.close();
	return file;
}

void write(string fileName, const string data) {
	ofstream outFile(fileName);
	//outFile << data << " ";
	outFile << data;
	outFile.close();
}

int main() {
	string toDecompress = read("compout");
	string decompressed = decompress(toDecompress);
	write("decompout", decompressed);
	return 0;
}