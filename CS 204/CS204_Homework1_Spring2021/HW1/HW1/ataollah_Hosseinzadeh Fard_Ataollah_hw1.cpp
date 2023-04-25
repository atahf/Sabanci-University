#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

//this function aims to clear extra spaces and replace \t with space.
void strip_white_space(string & s) {
	string tmp = "";
	char p_ch = ' ';
	for (int i = 0; i < s.length(); i++) {
		if ((s.at(i) == ' ' || s.at(i) == '\t') && (p_ch == ' ' || p_ch == '\t')) {}
		else if (s.at(i) == '\t' && (p_ch != ' ' || p_ch != '\t')) {
			p_ch = s.at(i);
			tmp += ' ';
		}
		else {
			tmp += s.at(i);
			p_ch = s.at(i);
		}
	}
	s = tmp;
}

//this function aims to print a 2D vector (matrix)  in matrix view.
void print(vector<vector<char>> matrix) {
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[i].size(); j++) {
			cout << matrix[i][j] << "\t";
		}
		cout << endl;
	}
}

int main() {
	//the first part will get name of the file and in the case of wrong name, it will keep asking the name till the right name.
	string fName;
	cout << "Please enter the name of the file: ";
	cin >> fName;
	ifstream textFile;
	textFile.open(fName.c_str());
	while (textFile.fail()) {
		cout << "File name is incorrect, please enter again: ";
		cin >> fName;
		textFile.open(fName.c_str());
	}
	cout << endl;
	int lineCount = 0;
	vector <string> textLines;
	string tempLine;
	while (getline(textFile, tempLine)) {
		strip_white_space(tempLine);
		textLines.push_back(tempLine);
		lineCount++;
	}
	textFile.close();

	//the second part will extract and analyze data from txt and will do input checks.
	string rowNcolumn = textLines[0];
	int row = stoi(rowNcolumn.substr(0, rowNcolumn.find(' '))), column = stoi(rowNcolumn.substr(rowNcolumn.find(' ')));

	//next line will check whether row and columns are positive and non-zero or not and if they are not then will display error and terminate program.
	if (row < 0 || column < 0) {
		cout << "Invalid number for column and/or column!\n" << endl;
	}
	else {
		vector<vector<char>> matrix(row, vector<char>(column, '-'));
		for (int l = 1; l < lineCount; l++) {
			string tmp;
			vector<string> data;
			int counter = 0;
			istringstream in(textLines[l]);
			while (getline(in, tmp, ' ')) {
				data.push_back(tmp);
				counter++;
			}

			//during gaining data from txt it will count the elements in each single line and if number of elements is not 5 in line it will display error and skip to the next line.
			if (counter != 5) {
				cout << "Invalid line! Number of values is different than 5." << endl << endl;
			}
			else {
				string word = data[0], dir = data[3], ori = data[4];
				int row = stoi(data[1]), column = stoi(data[2]);
				const int ROW = row, COL = column;
				bool clear = true;
				
				//following part is the input check part and echa if is for different error check and in case of existence of any errors will skip to the next line.
				if (row < 0 || row >= matrix.size() || column < 0 || column >= matrix[0].size()) {
					cout << "Starting point is out of range! Point: " << row << "," << column << endl << endl;		//out of range error
					clear = false;
				}
				else if (dir != "l" && dir != "u" && dir != "r" && dir != "d") {
					cout << "Invalid input for direction! Direction: " << dir << endl << endl;		//invalid direction error
					clear = false;
				}
				else if (ori != "CW" && ori != "CCW") {
					cout << "Invalid input for orientation! Orientation: " << ori << endl << endl;		//invalid orientation error
					clear = false;
				}
				if (clear == true) {

					/* In case that there is no input validation error program will start working on matrix.

					****this part of code is an optimized code that is why program does not have different parts for each type of
						orientation, there was a pattern between same directions but different orientations, so I have used that
						pattern and minimized the number of lines of code. */

					int placed = 0;
					bool result = false;

					/*in case that row and column given in each line is out of range of the main matrix, program will skip following and in the end of process of each line it will diplay the result.
					
					if the row and columns are in range, then program will start and first will place the first letter in given coordinates, the for the next letters wil decide what to do according to direction and 
					orientation and according to indexes around them.*/

					if (matrix[row][column] == '-') {
						vector<vector<char>> tmpM = matrix;
						for (int i = 0; i < word.length(); i++) {
							if (i == 0) {
								tmpM[row][column] = word.at(i);
								placed++;
							}
							else {
								if (dir == "r") {
									if (column < tmpM[0].size() - 1 && tmpM[row][column + 1] == '-') {
										column++;
										tmpM[row][column] = word.at(i);
										placed++;
									}
									else if ((ori == "CW" && row < tmpM.size() - 1 && tmpM[row + 1][column] == '-') || (ori == "CCW" && row > 0 && tmpM[row - 1][column] == '-')) {
										if(ori == "CW") {
											row++;
										}
										else{
											row--;
										}
										tmpM[row][column] = word.at(i);
										placed++;
									}
									else if (column > 0 && tmpM[row][column - 1] == '-') {
										column--;
										tmpM[row][column] = word.at(i);
										placed++;
									}
									else if ((ori == "CW" && row > 0 && tmpM[row - 1][column] == '-') || (ori == "CCW" && row < tmpM.size() - 1 && tmpM[row + 1][column] == '-')) {
										if(ori == "CW") {
											row--;
										}
										else{
											row++;
										}
										tmpM[row][column] = word.at(i);
										placed++;
									}
								}
								else if (dir == "l") {
									if (column > 0 && tmpM[row][column - 1] == '-') {
										column--;
										tmpM[row][column] = word.at(i);
										placed++;
									}
									else if ((ori == "CW" & row > 0 && tmpM[row - 1][column] == '-') || (ori == "CCW" & row < tmpM.size() - 1 && tmpM[row + 1][column] == '-')) {
										if(ori == "CW") {
											row--;
										}
										else{
											row++;
										}
										tmpM[row][column] = word.at(i);
										placed++;
									}
									else if (column < tmpM[0].size() - 1 && tmpM[row][column + 1] == '-') {
										column++;
										tmpM[row][column] = word.at(i);
										placed++;
									}
									else if ((ori == "CW" && row < tmpM.size() - 1 && tmpM[row + 1][column] == '-') || (ori == "CCW" && row > 0 && tmpM[row - 1][column] == '-')) {
										if(ori == "CW") {
											row++;
										}
										else{
											row--;
										}
										tmpM[row][column] = word.at(i);
										placed++;
									}
								}
								else if (dir == "u") {
									if (row > 0 && tmpM[row - 1][column] == '-') {
										row--;
										tmpM[row][column] = word.at(i);
										placed++;
									}
									else if ((ori == "CW" && column < tmpM[0].size() - 1 && tmpM[row][column + 1] == '-') || (ori == "CCW" && column > 0 && tmpM[row][column - 1] == '-')) {
										if(ori == "CW") {
											column++;
										}
										else{
											column--;
										}
										tmpM[row][column] = word.at(i);
										placed++;
									}
									else if (row < tmpM.size() - 1 && tmpM[row + 1][column] == '-') {
										row++;
										tmpM[row][column] = word.at(i);
										placed++;
									}
									else if ((ori == "CW" && column > 0 && tmpM[row][column - 1] == '-') || (ori == "CCW" && column < tmpM[0].size() - 1 && tmpM[row][column + 1] == '-')) {
										if(ori == "CW") {
											column--;
										}
										else{
											column++;
										}
										tmpM[row][column] = word.at(i);
										placed++;
									}
								}
								else if (dir == "d") {
									if (row < tmpM.size() - 1 && tmpM[row + 1][column] == '-') {
										row++;
										tmpM[row][column] = word.at(i);
										placed++;
									}
									else if ((ori == "CW" && column > 0 && tmpM[row][column - 1] == '-') || (ori == "CCW" && column < tmpM[0].size() - 1 && tmpM[row][column + 1] == '-')) {
										if(ori == "CW") {
											column--;
										}
										else{
											column++;
										}
										tmpM[row][column] = word.at(i);
										placed++;
									}
									else if (row > 0 && tmpM[row - 1][column] == '-') {
										row--;
										tmpM[row][column] = word.at(i);
										placed++;
									}
									else if ((ori == "CW" && column < tmpM[0].size() - 1 && tmpM[row][column + 1] == '-') || (ori == "CCW" && column > 0 && tmpM[row][column - 1] == '-')) {
										if(ori == "CW") {
											column++;
										}
										else{
											column--;
										}
										tmpM[row][column] = word.at(i);
										placed++;
									}
								}
							}
						}

						//in case that all letters of each word is placed to matrix it will sync main matrix with tempM matrix.
						if (placed == word.length()) {
							matrix = tmpM;
							result = true;
						}
					}

					//the following lines are the result displayment lines which will print matrix and result whether word is placed or not.
					string massage = (result)? "was": "could not be";
					cout << "\"" << word << "\" " << massage << " put into the matrix with given starting point: " << ROW << "," << COL << "\n" << "direction: " << dir << "\torientation: " << ori << endl;
					print(matrix);
					cout << endl;
				}
			}
		}
	}
	return 0;
}
//Ataollah Hosseinzadeh Fard
//ID: 28610