#include <iostream>
#include <string>

using namespace std;

bool source_checker(string source) {
	bool result = true;
	int len = source.length();
	for (int i = 0; i < len; i++) {
		if (source.at(i) < 32) {
			result = false;
		}
		else if (source.at(i) > 32 && source.at(i) < 48) {
			result = false;
		}
		else if (source.at(i) > 57 && source.at(i) < 65) {
			result = false;
		}
		else if (source.at(i) > 90 && source.at(i) < 97) {
			result = false;
		}
		else if (source.at(i) > 122) {
			result = false;
		}
	}
	return result;
}

void get_source(string & source) {
	string step;
	while (true) {
		cout << "Enter source string: ";
		while (cin >> step) {
			if (step == "end" || step == "END") {
				break;
			}
			source += step + " ";
		}
		if (source_checker(source) == true) {
			break;
		}
		else {
			source = "";
			cin.clear();
		}
	}
	source = source.substr(0, source.length() - 1);
}

string word_extract(string source, int index) {
	string result;
	int start, end;
	start = source.rfind(" ", index);
	end = source.find(" ", index) - 1;
	result = source.substr((start + 1), (end - start));
	return result;
}

void find_search_operator(string & search, string & search_operator) {
	if (search.at(search.length() - 1) == '+') {
		search_operator = "+";
		search = search.substr(0, search.length() - 1);
	}
	else if (search.at(search.length() - 1) == '.') {
		search_operator = ".";
		search = search.substr(0, search.length() - 1);
	}
	else if (search.at(search.length() - 1) == '*') {
		if (search.substr(search.find("*"), 2) == "**") {
			search_operator = "**";
			search = search.substr(0, search.length() - 2);
		}
		else {
			search_operator = "*";
			search = search.substr(0, search.length() - 1);
		}
	}
}

bool never_happened_BoA(string source, string search, string word, int index) {
	bool result = true;
	if (source.find(" ", index) == index + search.length() + 1 && search == source.substr(index + search.length(), search.length())) {
		result = false;
	}
	if ((source.rfind(" ", index) == index - search.length() - 1) && search == source.substr(index - search.length(), search.length())) {
		result = false;
	}
	return result;
}

void outcome(string source, string word, string search, int index, string search_operator) {
	if (search_operator == "*" && never_happened_BoA(source, search, word, index) == true && (index != 0 && source.at(index - 1) != ' ') && ((source.find(" ", index) != string::npos || search != source.substr((source.length() - search.length()), search.length())) && source.find(" ", index) != (index + search.length()))) {
		cout << "index: " << index << " word: " << word << endl;
	}
	else if (search_operator == "." && ((source.find(" ", index) == string::npos && search == source.substr((source.length() - search.length()), search.length())) || source.find(" ", index) == (index + search.length()))) {
		cout << "index: " << index << " word: " << word << endl;
		
	}
	else if (search_operator == "+" && (index == 0 || source.at(index - 1) == ' ')) {
		cout << "index: " << index << " word: " << word << endl;
	}
	else if (search_operator == "**") {
		cout << "index: " << index << " word: " << word << endl;
	}
}

void search(string source) {
	while (true) {
		string search, sub_source, search_operator = "", word;
		int index = 0;
		cout << "Enter search string: ";
		cin >> search;
		find_search_operator(search, search_operator);
		sub_source = " " + source + " ";
		if (search == "quit" || search == "QUIT") {
			break;
		}
		else if (source.find(search) != string::npos && search_operator.length() != 0) {
			for (int i = 0; i < source.length();) {
				if (sub_source.find(search) != string::npos) {
					index += sub_source.find(search) - 1;
					word = word_extract(source, index);
					outcome(source, word, search, index, search_operator);
					sub_source = "  " + sub_source.substr(sub_source.find(search) + 1);
					i = i + 1 + sub_source.find(search);
				}
				else {
					break;
				}
			}
		}
		cin.clear();
	}
}

int main() {
	string source;
	get_source(source);
	search(source);
	return 0;
}

//Ataollah Hosseinzadeh Fard
//ID: 28610
