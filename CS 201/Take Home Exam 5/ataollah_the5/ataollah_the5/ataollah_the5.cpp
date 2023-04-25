#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

struct TV_shows {
	string show_name, actor_name;
	double rating, sum;
	int count;
};

string To_Upper(string str) {
	string res, temp;
	char ch, p_ch;
	istringstream in(str);
	while (in.get(ch)) {
		if (ch >= 'a' and ch <= 'z') {
			ch = toupper(ch);
			res += ch;
			p_ch = ch;
		}
		else if (p_ch == char(32) && p_ch == ch) {}
		else {
			res += ch;
			p_ch = ch;
		}
	}
	return res;
}

string gain_text(ifstream & file) {
	char ch, p_ch = ' ';
	string text = "";
	while (file.get(ch)) {
		if (ch >= 'a' and ch <= 'z') {
			ch = toupper(ch);
			text += ch;
			p_ch = ch;
		}
		else if (p_ch == char(32) && p_ch == ch) {

		}
		else {
			text += ch;
			p_ch = ch;
		}
	}
	return text;
}

string extract_words(string line) {
	string res, word;
	istringstream in_line(line);
	while (in_line >> word) {
		res += word + " ";
	}
	return res.substr(0, res.length() - 1);
}

bool existsWord(const vector<TV_shows> & a, const string s1, const string s2)
{
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i].show_name == s2 && a[i].actor_name == s1)
			return true;
	}
	return false;
}

void Sort_Ratings(vector<TV_shows> & a) {
	bool sorted = true;
	for (int i = 1; i < a.size() && sorted; i++) {
		if (a[i - 1].rating > a[i].rating) {
			sorted = false;
		}
	}
	if (!sorted) {
		int j, k, minIndex, numElts = a.size();
		TV_shows temp;
		for (k = 0; k < numElts - 1; k++) {
			minIndex = k;
			for (j = k + 1; j < numElts; j++)
				if (a[j].rating > a[minIndex].rating)
					minIndex = j;
			temp.rating = a[k].rating;
			a[k].rating = a[minIndex].rating;
			a[minIndex].rating = temp.rating;

			temp.show_name = a[k].show_name;
			a[k].show_name = a[minIndex].show_name;
			a[minIndex].show_name = temp.show_name;

			temp.actor_name = a[k].actor_name;
			a[k].actor_name = a[minIndex].actor_name;
			a[minIndex].actor_name = temp.actor_name;
		}
	}
}

void Sort_Alphabet(vector<TV_shows> & a) {
	bool sorted = true;
	for (int i = 1; i < a.size() && sorted; i++) {
		if (a[i - 1].show_name > a[i].show_name) {
			sorted = false;
		}
	}
	if (!sorted) {
		int j, k, minIndex, numElts = a.size();
		TV_shows temp;
		for (k = 0; k < numElts - 1; k++) {
			minIndex = k;
			for (j = k + 1; j < numElts; j++)
				if (a[j].show_name < a[minIndex].show_name && a[j].rating == a[minIndex].rating)
					minIndex = j;
			temp.rating = a[k].rating;
			a[k].rating = a[minIndex].rating;
			a[minIndex].rating = temp.rating;

			temp.show_name = a[k].show_name;
			a[k].show_name = a[minIndex].show_name;
			a[minIndex].show_name = temp.show_name;

			temp.actor_name = a[k].actor_name;
			a[k].actor_name = a[minIndex].actor_name;
			a[minIndex].actor_name = temp.actor_name;
		}
	}
}

int main() {
	string a_name, r_name, a_text, r_text, a_line, a1, a2, r_line, sub_line, r2, final, t_fav, fav, choice;
	char ch, p_ch;
	double r1, min;
	int pos, index;
	bool _continue, favv;
	TV_shows temp, f_temp;
	ifstream a_input, r_input;
	vector<TV_shows> tvShows, favs;
	cout << "Welcome to Netflix TV Series Ratings Calculator!" << endl;
	cout << "Please enter a filename for Series-Actor file: ";
	cin >> a_name;
	a_input.open(a_name.c_str());
	while (a_input.fail()) {
		cout << "Can not find the specified file." << endl;
		cout << "Please enter a filename for Series-Actor file: ";
		cin >> a_name;
		a_input.open(a_name.c_str());
	}
	cout << "Please enter a filename for Rating file: ";
	cin >> r_name;
	r_input.open(r_name.c_str());
	while (r_input.fail()) {
		cout << "Can not find the specified file." << endl;
		cout << "Please enter a filename for Rating file: ";
		cin >> r_name;
		r_input.open(r_name.c_str());
	}
	a_text = gain_text(a_input);
	r_text = gain_text(r_input);
	istringstream a_lines(a_text);
	while (getline(a_lines, a_line)) {
		pos = a_line.find(";");
		a1 = a_line.substr(0, pos);
		a1 = extract_words(a1);
		a2 = a_line.substr(pos + 1);
		a2 = extract_words(a2);
		temp.actor_name = a1;
		temp.show_name = a2;
		temp.count = 0;
		temp.rating = 0;
		temp.sum = 0;
		if (!existsWord(tvShows, a1, a2)) {
			tvShows.push_back(temp);
		}
	}
	istringstream r_lines(r_text);
	while (getline(r_lines, r_line)) {
		final = "";
		istringstream in(r_line);
		while (in >> sub_line) {
			final += sub_line + " ";
		}
		final = final.substr(0, final.length() - 1);
		pos = final.find(" ");
		r1 = stod(final.substr(0, pos));
		r2 = final.substr(pos + 1);
		if (tvShows.size() > 0) {
			for (int i = 0; i < tvShows.size(); i++) {
				if (tvShows[i].show_name == r2) {
					tvShows[i].sum += r1;
					tvShows[i].count++;
				}
			}
		}
	}

	if (tvShows.size() > 0) {
		for (int i = 0; i < tvShows.size(); i++) {
			tvShows[i].rating = tvShows[i].sum / tvShows[i].count;
		}
	}
	Sort_Ratings(tvShows);
	Sort_Alphabet(tvShows);
	_continue = true;
	while (_continue) {
		cout << "---" << endl;
		cout << "MENU\n1. Print the series\n2. Search the series according to the rating\n3. Add the series to the favorite list\n4. Print the favorite list\n5. Exit" << endl;
		cout << "---" << endl;
		cout << "Enter your choice: ";
		cin >> choice;
		cout << "---" << endl;
		if (choice == "1") {
			cout << "RANK,NAME,ACTOR,POINTS" << endl;
			for (int i = 0; i < tvShows.size(); i++) {
				cout << i + 1 << "," << tvShows[i].show_name << "," << tvShows[i].actor_name << "," << tvShows[i].rating << endl;
			}
		}
		else if (choice == "2") {
			cout << "Please enter the minimum rating: ";
			cin >> min;
			if (0 <= min && min <= 10){
				cout << "RANK,NAME,ACTOR,POINTS" << endl;
				for (int i = 0; i < tvShows.size(); i++) {
					if (tvShows[i].rating >= min) {
						cout << i + 1 << "," << tvShows[i].show_name << "," << tvShows[i].actor_name << "," << tvShows[i].rating << endl;
					}
				}
			}
			else {
				cout << "This is not a valid rating!" << endl;
			}
		}
		else if (choice == "3") {
			favv = false;
			fav = "";
			cout << "Please enter the name of series: ";
			for (int i = 0; i <= 1; i++) {
				getline(cin, t_fav);
				fav = To_Upper(t_fav);
			}
			for (int i = 0; i < tvShows.size() && !favv; i++) {
				if (tvShows[i].show_name == fav) {
					favv = true;
				}
			}
			if (!favv) {
				cout << "There is no such TV series!" << endl;
			}
			else {
				if (favs.size() > 0) {
					for (int i = 0; i < favs.size() && favv; i++) {
						if (favs[i].show_name == fav) {
							cout << "Your favorite list already have " << fav << endl;
							favv = false;
						}
					}
				}
				if (favv) {
					for (int i = 0; i < tvShows.size() - 1, favv; i++) {
						if (tvShows[i].show_name == fav) {
							f_temp.actor_name = tvShows[i].actor_name;
							f_temp.show_name = tvShows[i].show_name;
							f_temp.rating = tvShows[i].rating;
							favv = false;
						}
					}
					favs.push_back(f_temp);
				}
			}
		}
		else if (choice == "4") {
			if (favs.size() > 0) {
				cout << "RANK,NAME,ACTOR,POINTS" << endl;
				int j = 1;
				for (int i = favs.size() - 1; i >= 0; i--) {
					cout << j << "," << favs[i].show_name << "," << favs[i].actor_name << "," << favs[i].rating << endl;
					j++;
				}
			}
			else {
				cout << "Your favorite list is currently empty!" << endl;
			}
		}
		else if (choice == "5") {
			_continue = false;
		}
		else {
			cout << "This is not a valid option!" << endl;
		}
	}
	return 0;
}

//Ataollah Hosseinzadeh Fard
//ID: 28610
