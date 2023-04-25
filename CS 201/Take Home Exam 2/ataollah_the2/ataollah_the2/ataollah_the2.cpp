#include <iostream>
#include <iomanip>
#include <string>
#include "strutils.h"

using namespace std;

//Function1 (Boolean) check_input
bool check_input(string color, double width, double height, double length) {
	bool result = false;
	if (width <= 0.0 || height <= 0.0 || length <= 0.0) {
		cout << "Invalid entry for " << color << " cuboid. Dimensions should be positive real numbers!" << endl;
	}
	else {
		result = true;
	}
	return result;
}

//Function2 (viod) checkCubeanDisplay
void checkCubeanDisplay(string color, double width, double height, double length) {
	string result_cube;
	if (width == height && height == length) {
		result_cube = "a cube.";
	}
	else {
		result_cube = "not a cube.";
	}
	cout << color << " cuboid is " << result_cube << endl;
	cout << "" << endl;
}

//Function3 (void) display
void display(string color, double area, double volume) {
	cout << "Results for the " << color << " cuboid:" << endl;
	cout << "Area: " << area << endl;
	cout << "Volume: " << volume << endl;
}

//Function4 (void) calculate
void calculate(string color, double width, double height, double length, double & area, double & volume) {
	volume = height * width * length;
	area = 2 * (height * width + width * length + height * length);
	display(color, area, volume);
	checkCubeanDisplay(color, width, height, length);
}

//Function5 (string) compare
string compare(string color1, string color2, string choice, double choice1, double choice2) {
	string massage;
	if (choice1 > choice2) {
		massage = color1 + " has the largest " + choice + ".";
	}
	else if (choice1 < choice2) {
		massage = color2 + " has the largest " + choice + ".";
	}
	else {
		massage = "Two cuboids have the same " + choice + ".";
	}
	return massage;
}

int main() {
	double w1, h1, l1, w2, h2, l2, area1, volume1, area2, volume2;
	string color1, color2,color1U, color2U, comparisonA, comparisonV;
	cout.setf(ios::fixed);
	cout.precision(2);
	cout << "Hello! This program compares two cuboids..." << endl;
	cout << "Please enter colors for the two cuboids: ";
	cin >> color1 >> color2;
	color1U = color1;
	color2U = color2;
	ToUpper(color1U);
	ToUpper(color2U);
	if (color1U == color2U) {
		cout << "Color names cannot be the same, good bye..." << endl;
	}
	else {
		cout << "Please enter length, width and height of the " << color1 << " cuboid: ";
		cin >> w1 >> h1 >> l1;
		if (check_input(color1, w1, h1, l1)) {
			cout << "Please enter length, width and height of the " << color2 << " cuboid: ";
			cin >> w2 >> h2 >> l2;
			if (check_input(color2, w2, h2, l2)) {
				calculate(color1, w1, h1, l1, area1, volume1);
				calculate(color2, w2, h2, l2, area2, volume2);
				cout << "Comparison of the two cuboids:" << endl;
				comparisonA = compare(color1, color2, "volume", volume1, volume2);
				comparisonV = compare(color1, color2, "area", area1, area2);
				cout << comparisonA << endl;
				cout << comparisonV << endl;
			}
		}
	}
	return 0;
}

//Ataollah Hosseinzadeh Fard
//28610
