#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <climits>

using namespace std;

void file_opener(ifstream & file, string & file_name, string fullFileName) {
	cout << "Please enter a filename for the " << fullFileName << ": ";
	cin >> file_name;
	file.open(file_name.c_str());
	while (file.fail()) {
		cout << "Cannot open the " << fullFileName << " file." << endl;
		cout << "Please enter a filename for the " << fullFileName << ": ";
		cin >> file_name;
		file.open(file_name.c_str());
	}
}

string gain_text(ifstream & file) {
	char ch, p_ch = ' ';
	string text = "";
	while (file.get(ch)) {
		if (ch >= 'A' and ch <= 'Z') {
			ch = 'a' + (ch - 'A');
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

string minimize_mistakes(string word, string sentence) {
	string step, result = "";
	int min = INT_MAX;
	istringstream in(sentence);
	while (in >> step) {
		if (word == step) {
			result = step;
			break;
		}
		else {
			int max_mistake = word.length() / 2, mistake = 0;
			for (int i = 0; i < word.length() && mistake <= max_mistake; i++) {
				if (word.at(i) != step.at(i)) {
					mistake++;
				}
			}
			if (mistake <= max_mistake) {
				if (mistake < min) {
					min = mistake;
					result = step + ",";
				}
				else if (mistake == min) {
					result += step + ",";
				}
			}			
		}
	}
	if (result == "") {
		result = "{" + word + "}";
	}
	else if (result.find(',') != string::npos) {
		result = result.substr(0, result.length() - 1);
		if (result.find(',') != string::npos) {
			result = "(" + result + ")";
		}
	}
	return result;
}

string spell_correction(string pool_text, string text_text) {
	string word_p, word_t, line_result = "", step_result, result = "";
	istringstream t_word_o(text_text);
	int i = 0;
	while (t_word_o >> word_t) {
		istringstream p_word_o(pool_text);
		step_result = "";
		
		while (p_word_o >> word_p) {
			if (word_t.length() == word_p.length()) {
				step_result += word_p + " ";
			}
		}
		result += minimize_mistakes(word_t, step_result) + " ";
	}
	return result.substr(0, result.length() - 1);
}

void print(const string & text_text, const string & result_line) {
	string result = "", line, word, ans = result_line, line_res;
	istringstream in_line(text_text);
	cout << "You may find the results below:\n" << endl;
	while (getline(in_line, line)) {
		line_res = "";
		if (line == "") {
			cout << "\n" << endl;
		}
		else {
			istringstream words(line);
			while (words >> word) {
				if (ans.find(" ") == string::npos) {
					line_res += ans + " ";
				}
				else {
					line_res += ans.substr(0, ans.find(" ")) + " ";
					ans = ans.substr(ans.find(" ") + 1);
				}
			}
			cout << line_res.substr(0, line_res.length() - 1) << endl;
		}
	}
}

int main() {
	string p_name, t_name;
	ifstream pool, text;
	file_opener(pool, p_name, "word pool");
	file_opener(text, t_name, "main text");
	string pool_text = gain_text(pool), text_text = gain_text(text), results = spell_correction(pool_text, text_text);
	print(text_text, results);
	pool.close();
	text.close();
	return 0;
}

//Ataollah Hosseinzadeh Fard
//ID: 28610