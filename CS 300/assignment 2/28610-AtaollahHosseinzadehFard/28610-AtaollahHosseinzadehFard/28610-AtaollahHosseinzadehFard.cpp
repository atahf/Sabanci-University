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

string compress(const string s) {
	string res = "", tmp;
	const dict<string> error = dict<string>("-1", -1);
	dict<string> tmpPair = dict<string>("-1", -1);
	dict<string> prevMatch = dict<string>("-1", -1);
	HashTable<dict<string>> T(error, 4096);
	for(int i = 1; i < 256; i++){
		tmpPair.data = (unsigned char) i;
		tmpPair.code = T.getCurrentSize() + 1;
		T.insert(tmpPair);
	}
	tmp = s;
	while(tmp.length() > 0) {
		for(int i = 1; i <= tmp.length(); i++) {
			string subTmp = tmp.substr(0, i);
			tmpPair.data = subTmp;
			if(T.find(tmpPair) == error) {
				res += to_string(prevMatch.code) + " ";
				dict<string> tmpDict = dict<string>(subTmp, T.getCurrentSize() + 1);
				if(T.getCurrentSize() < 4096)
					T.insert(tmpDict);
				tmp = tmp.substr(i - 1);
				break;
			}
			prevMatch = T.getElement(T.getIndex(tmpPair));
		}
		tmpPair.data = tmp;
		if(T.find(tmpPair) != error) {
			res += to_string(T.getElement(T.getIndex(T.find(tmpPair))).code);
			tmp = "";
		}
	}
	return res;
};

string ddecompress(const string s) {
	string res = "", tmp;
	const dict<string> error = dict<string>("-1", -1);
	dict<string> newKey = dict<string>("-1", -1);
	dict<string> response = dict<string>("-1", -1);
	dict<string> prevResponse = dict<string>("-1", -1);
	int idx;
	HashTable<dict<string>> T(error, 4096);
	for(int i = 1; i < 256; i++){
		newKey.data = (unsigned char) i;
		newKey.code = T.getCurrentSize() + 1;
		T.insert(newKey);
	}
	istringstream it(s);
	while(it >> tmp) {
		idx = stoi(tmp);
		response = T.get(idx);
		if(response != error) {
			res += response.data;
			if(prevResponse != error) {
				newKey.data = prevResponse.data + response.data.substr(0, 1);
				newKey.code = T.getCurrentSize() + 1;
				if(T.getCurrentSize() < 4096)
					T.insert(newKey);
			}
			prevResponse.code = response.code;
			prevResponse.data = response.data;
		}
		else {
			newKey.data = prevResponse.data + prevResponse.data.substr(0, 1);
			newKey.code = T.getCurrentSize() + 1;
			res += newKey.data;
			prevResponse.data = newKey.data;
			prevResponse.code = newKey.code;
			if(T.getCurrentSize() < 4096)
				T.insert(newKey);
		}
	}
	return res;
};

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
	outFile << data;
	outFile.close();
}

int main() {
	string toCompress = read("compin");
	string compressed = compress(toCompress);
	write("compout", compressed);

	string toDecompress = read("compout");
	string decompressed = decompress(toDecompress);
	write("decompout", decompressed);

	return 0;
}