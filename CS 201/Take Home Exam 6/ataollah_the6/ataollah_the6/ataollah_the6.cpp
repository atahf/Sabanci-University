#include <iostream>
#include <fstream>
#include "favoriteseries.h"

using namespace std;

void file_opener(ifstream & file, string file_name, string & text) {
	file.open(file_name.c_str());
	char ch, p_ch = ' ';
	while (file.get(ch)) {
		if (ch >= 'a' and ch <= 'z') {
			ch = toupper(ch);
			text += ch;
			p_ch = ch;
		}
		else if (p_ch == char(32) && p_ch == ch) {}
		else if (ch == '\t') {
			p_ch = ' ';
		}
		else {
			text += ch;
			p_ch = ch;
		}
	}
	file.close();
}

int _find(const vector<series> & v, int value) {
	for (int i = 0; i < v.size(); i++) {
		if (v[i].seriesCode == value) {
			return i;
		}
	}
	return -1;
}

void _main(vector<series> & allSeries, const string S_text, const string E_text) {
	string line, word;
	istringstream in_line(S_text);
	while (getline(in_line, line)) {
		int pos = line.find(" "), posS;
		vector<int> posC;
		series temp;
		for (int i = 0; i < line.length(); i++) {
			if (line.at(i) == ';') {
				posS = i;
				posC.push_back(i);
			}
			else if (line.at(i) == ',') {
				posC.push_back(i);
			}
		}
		temp.seriesCode = stoi(line.substr(0, pos));
		temp.seriesName = line.substr(pos + 1, posS - pos - 1);
		stringstream in1(line.substr(posS + 1));
		string names;

		while (getline(in1, names, ',')) {
			temp.actorNames.push_back(names.substr(1, names.length() - 1));
		}
		allSeries.push_back(temp);
	}
	int place;
	string linee, elem;
	istringstream in_linee(E_text);
	while (getline(in_linee, linee)) {
		int counter = 0;
		episode tempp;
		tempp.episodeName = "";
		stringstream in2(linee);
		while (getline(in2, elem, ' ')) {
			if (counter == 0) {
				tempp.seriesCode = stoi(elem);
				place = _find(allSeries, stoi(elem));
			}
			else if (counter == 1) {
				int m, d, y;
				m = stoi(elem.substr(0, elem.find('/')));
				d = stoi(elem.substr(elem.find('/') + 1, elem.rfind('/')));
				y = stoi(elem.substr(elem.rfind('/') + 1));
				tempp.date = Date(m, d, y);
			}
			else if (counter == 2) {
				tempp.slot = elem;
			}
			else {
				tempp.episodeName += elem + " ";
			}
			counter++;
		}
		tempp.episodeName = tempp.episodeName.substr(0, tempp.episodeName.length() - 1);
		StripPunc(tempp.episodeName);
		StripWhite(tempp.episodeName);
		allSeries[place].episodes.push_back(tempp);
	}
}

int main() {
	string nameF, nameL, choice, serie = "", episodes = "";
	bool _continue = true;
	ifstream S, E;
	file_opener(S, "series.txt", serie);
	file_opener(E, "episodes.txt", episodes);
	cout << "Welcome to my favorite TV series schedule program!\nPlease, enter your name and surname: ";
	cin >> nameF >> nameL;
	FavoriteSeries f(nameF, nameL);
	vector<series> v;
	vector<TV_shows> tvShows;
	TV_shows temp;
	_main(v, serie, episodes);
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].episodes.size(); j++) {
			temp.actors = v[i].actorNames;
			temp.code = v[i].seriesCode;
			temp.date = v[i].episodes[j].date;
			temp.nameS = v[i].seriesName;
			temp.nameE = v[i].episodes[j].episodeName;
			temp.slot = v[i].episodes[j].slot;
			tvShows.push_back(temp);
		}
	}
	f.sort(tvShows);
	while (_continue) {
		cout << "---\nMENU\n1. Print all TV series\n2. Add TV series\n3. Drop TV series\n4. Print your favorite series with schedule\n5. Print all your favorite actors\n6. Exit\n---\nPlease enter your choice: ";
		cin >> choice;
		if (choice == "1") {
			cout << "CODE,SERIES,EPISODE,DATE,SLOT" << endl;
			for (int i = 0; i < tvShows.size(); i++) {
				cout << tvShows[i].code << "," << tvShows[i].nameS << "," << tvShows[i].nameE << "," << tvShows[i].date << "," << tvShows[i].slot << endl;
			}
		}
		else if (choice == "2") {
			int code;
			cout << "Enter code for the TV series you want to add: ";
			cin >> code;
			if (f.ifExists(code)) {
				cout << f.getFullName() << ", you can't add this TV series because you already added it!" << endl;
			}
			else {
				bool exist = false;
				for (int i = 0; i < v.size() && !exist; i++) {
					if (v[i].seriesCode == code) {
						exist = true;
					}
				}
				if (!exist) {
					cout << f.getFullName() << ", there is no such TV series in the database!" << endl;
				}
				else {
					for (int i = 0; i < v.size(); i++) {
						if (v[i].seriesCode == code) {
							if (f.isConflict(v[i])) {
								cout << f.getFullName() << ", you can't add this TV series because of a date and slot conflict!" << endl;
							}
							else {
								f.addSeries(v[i]);
								cout << f.getFullName() << ", " << v[i].seriesName << " added to your schedule." << endl;
							}
						}
					}
				}
			}
		}
		else if (choice == "3") {
			if (f.isListEmpty()) {
				cout << f.getFullName() << ", you did not add any TV series yet!" << endl;
			}
			else {
				int codeDrop;
				cout << "Enter code for the TV series you want to drop: ";
				cin >> codeDrop;
				if (!f.ifExists(codeDrop)) {
					cout << f.getFullName() << ", there is no such TV series in your favorite list!" << endl;
				}
				else {
					cout << f.getFullName() << ", " << f.getSeriesName(codeDrop) << " has been dropped from your favorite list!" << endl;
					f.dropSeriesSorted(codeDrop);
				}
			}
		}
		else if (choice == "4") {
			if (f.isListEmpty()) {
				cout << f.getFullName() << ", you did not add any TV series yet!" << endl;
			}
			else {
				cout << f.getFullName() << ", here is your favorite TV series:" << endl;
				f.displayFavoriteList();
			}
		}
		else if (choice == "5") {
			if (f.isListEmpty()) {
				cout << f.getFullName() << ", you did not add any TV series yet!" << endl;
			}
			else {
				cout << f.getFullName() << ", here is your favorite actors:" << endl;
				f.displayFavoriteActors();
			}
		}
		else if (choice == "6") {
			cout << "Goodbye, " << f.getFullName() << "!" << endl;
			_continue = false;
		}
		else {
			cout << "Invalid option!" << endl;
		}
	}
	return 0;
}
//Ataollah Hosseinzadeh Fard
//ID: 28610