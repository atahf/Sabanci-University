#include <iostream>
#include <fstream>
#include <string>
#include "ataollah_Hosseinzadeh Fard_Ataollah_hw4_stack.h"

using namespace std;

struct cellStruct {
	char value;
	bool flag;
};

/*getMatrix function takes row and column and ifstream as parameters 
  and creates a 2D dynamic array of cellStruct struct type and puts 
  correspunding values from ifstream's file to array.*/
cellStruct **getMatrix(int row, int column, ifstream &file) {
	cellStruct **matrix = new cellStruct* [row];
    for(int i = 0; i < row; i++)
        matrix[i] = new cellStruct[column];
	string line;
	for (int i = 0; i < row; i++) {
		getline(file, line);
		for(int j = 0; j < column; j++) {
			matrix[i][j].value = line.at(j);
			matrix[i][j].flag = false;
		}
	}
	return matrix;
}

/*findNplace function performs the main task of the HW4, it will try to find 
  the best folution coordinates for given search bit string, if it finds it 
  pushes the coordinates into the stack, otherwise the stack will be empty.*/
void findNplace (Stack &stack, cellStruct **&matrix, const string &search, const int &_row, const int &_column, bool &check) {
	if(search.at(0) != matrix[0][0].value)
		check = true;
	else {
		int row = 0, col = 0, counter = 1, size = search.length();
		stack.push(row, col);
		matrix[row][col].flag = true;
		while(row < _row && col < _column && counter < size) {
			if(row == _row - 1 && col == _column - 1) {
				check = true;
				return;
			}
			else {
				if(matrix[row][col + 1].flag == false && matrix[row][col + 1].value == search.at(counter)) {
					col++;
					counter++;
					stack.push(row, col);
					matrix[row][col].flag = true;
				}
				else {
					if(matrix[row + 1][col].flag == false && matrix[row + 1][col].value == search.at(counter)) {
						row++;
						counter++;
						stack.push(row, col);
						matrix[row][col].flag = true;
					}
					else {
						int tmpRow, tmpCol;
						stack.pop(tmpRow, tmpCol);
						if(stack.isEmpty()) {
							check = true;
							return;
						}
						stack.pop(tmpRow, tmpCol);
						stack.push(tmpRow, tmpCol);
						row = tmpRow;
						col = tmpCol;
						counter--;
					}
				}
			}
		}
		if(counter != size)
			check = true;
	}
}

/*resetFlags function basically makes all flags of 2D array false, this 
  is used when the search is done and we are about to get a new search 
  string but before getting we should undo all the changes made on array.*/
void resetFlags(cellStruct **&matrix, const int &row, const int &column) {
	for(int i = 0; i < row; i++)
		for (int j = 0; j < column; j++)
			matrix[i][j].flag = false;
}

/*printReverseStack function by poping from given stack and pushing values to 
  a temp stack and then by poping from temp stack it prints value in console.*/
void printReverseStack(Stack &stack) {
	if(stack.isEmpty()) {
		cout << "Something went wrong. Please try again!" << endl;
		return;
	}
	Stack tmp;
	int row, col;
	while(!stack.isEmpty()) {
		stack.pop(row, col);
		tmp.push(row, col);
	}
	while(!tmp.isEmpty()) {
		tmp.pop(row, col);
		cout << "(" << row << "," << col << ") ";
	}
	cout << endl;
}

int main() {
	//this part gets row, column, and file name from user.
	int row, column;
	string fileName, search;
	cout << "Please enter the number of rows: ";
	cin >> row;
	cout << "Please enter the number of columns: ";
	cin >> column;
	cout << "Please enter the name of the input file that contains the matrix: ";
	cin >> fileName;
	ifstream file;
	file.open(fileName.c_str());
	while (file.fail()) {
		cout << "File cannot be opened.\nPlease enter the name of the input file again: ";
		cin >> fileName;
		file.open(fileName.c_str());
	}

	//creates 2D array of cellStruct struct type with wanted row and column size.
	cellStruct **matrix = getMatrix(row, column, file);
	file.close();

	//inside loop the search bit string is entered by user and by findNplace function the results will be displayed.
	bool _continue = true;
	while(_continue) {
		Stack stack;
		cout << "\nPlease enter a string of bits to search (CTRL+Z to quit): ";
		cin >> search;

		//the loop untill user enters CTRL-Z the loop will continue until infinity.
		if(cin.eof())
			_continue = false;
		else {
			bool check = false;
			findNplace(stack, matrix, search, row, column, check);
			if (check)
				cout << "The bit string " << search << " could not be found." << endl;
			else {
				cout << "The bit string " << search << " is found following these cells:" << endl;
				printReverseStack(stack);
			}
			cout << "---------------------------------------------------------";
			cin.clear();

			//resetFlags is called to prepare array for next search.
			resetFlags(matrix, row, column);
		}
	}

	//2D array is properly deallocated and program ends.
	for (int i = 0; i < row; i++)
        delete[] matrix[i];
    delete[] matrix;
	cout << "Program ended successfully!\n" << endl;
	return 0;
}

//Ataollah Hosseinzadeh Fard
//ID: 28610