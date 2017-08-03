#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <conio.h>
//TODO: fix decimal displaying and calculation
//Current issue: numDigits() thinks all decimals are 15 digits long
using namespace std;

namespace shared {
	double matrix1[99][99];
	double matrix2[99][99];
	int matrix1Index[2], matrix2Index[2];
	double result[99][99];
	int resultIndex[2];
	int maxDigits = 0;
	bool indexSet = false;
}

int numDigits(double number) //measures number of digits in a number
{
	stringstream ss;
	ss.precision(15);
	ss << number;
	ss.seekg(0, ios::end);
	return ss.tellg();
}

double *parseText(string text, int matrixIndex[]) { //return array of all numbers in a string (row in a matrix)
	int tmpIndex;
	if (text.at(0) == ' ') {text.erase(0, 1); }
	if (text.at(text.size() - 1) == ' ') text.erase(text.size() - 1);
	text += ' ';
	string::size_type sz;
	int e, s = 0;
	double tmp[99];
	double *num = tmp;
	int pCol = 0;
	for (int i = 0; i < text.length(); i++) {
		if (text.at(i) == ' ') {
			e = i;
			num[pCol] = stof(text.substr(s, e - s), &sz);
			if (numDigits(num[pCol]) > shared::maxDigits) shared::maxDigits = numDigits(num[pCol]);
			if (!shared::indexSet) { matrixIndex[1] = pCol; }
			tmpIndex = pCol;
			pCol++;
			s = e + 1;
		}
	}
	if (tmpIndex != matrixIndex[1]) { cout << "ERROR: Number of columns does not match!\n"; }
	shared::indexSet = true;
	return num;
}


void cpRowToMatrix(double *row, int rowNum, double matrix[][99], int matrixIndex[]) { //copy the row to the matrix for use in later calulations
	for (int i = 0; i <= matrixIndex[1]; i++) {
		matrix[rowNum][i] = row[i];
	}
}

void multiplyMatrices() {
	shared::resultIndex[0] = shared::matrix1Index[0];
	shared::resultIndex[1] = shared::matrix2Index[1];
	for (int i = 0; i <= shared::matrix1Index[0]; i++) {
		for (int j = 0; j <= shared::matrix2Index[1]; j++) {
			for (int k = 0; k <= shared::matrix1Index[1]; k++) {
				shared::result[i][j] += shared::matrix1[i][k] * shared::matrix2[k][j];
				if (numDigits(shared::result[i][j]) > shared::maxDigits) shared::maxDigits = numDigits(shared::result[i][j]);
			}
		}
	}
}

void zeroResult() { //needed to make sure the += does not apply to random junk in memory space in multiplyMatrices()
	for (int i = 0; i < shared::resultIndex[0]; i++)
		for (int j = 0; j < shared::resultIndex[1]; j++)
		{
			shared::result[i][j] = 0;
		}
}

void printMatrix(double matrix[][99], int indexArray[2]) { //makes matrices look all fancy
	for (int h = 0; h <= ((indexArray[1] + 1) * (shared::maxDigits + 1)); h++) {
		cout << "-";
	}
	cout << "\n|";
	for (int i = 0; i <= indexArray[0]; i++) {
		for (int j = 0; j <= indexArray[1]; j++) {
			cout << matrix[i][j];
			for (int l = 0; l < shared::maxDigits - numDigits(matrix[i][j]); l++) {
				cout << " ";
			}
			cout << "|";
		}
		cout << "\n|";
		for (int h = 0; h <= ((indexArray[1] + 1) * (shared::maxDigits + 1)) - 3 && i < indexArray[0]; h++) {
			cout << "-";
		}
		if (i < indexArray[0]) { cout << "-|\n|"; }
	}
	cout << "\r";
	for (int h = 0; h <= ((indexArray[1] + 1) * (shared::maxDigits + 1)); h++) {
		cout << "-";
	}
	cout << "\n";
}

int main()
{
	double *tmpRow;
	string text;
	bool done = false;
	cout << "Separate numbers with spaces\nUse the return key with no input to finish the matrix\n*NOTE* Decimals in this program are approximated\nEnter first matrix\n" << endl;
	for (int r = 0; !done; r++) {
		getline(cin, text);
		if (text != "") {
			cpRowToMatrix(parseText(text, shared::matrix1Index), r, shared::matrix1, shared::matrix1Index);
			shared::matrix1Index[0] = r;
		}
		else { done = true; }
	}
	done = false;
	shared::indexSet = false;
	cout << "Enter next matrix\n";
	for (int r = 0; !done; r++) {
		getline(cin, text);
		if (text != "") {
			cpRowToMatrix(parseText(text, shared::matrix2Index), r, shared::matrix2, shared::matrix2Index);
			shared::matrix2Index[0] = r;
		}
		else { done = true; }
	}
	if (shared::matrix1Index[1] != shared::matrix2Index[0]) {
		cout << "WARNING, THIS OPERATION IS UNDEFINED. TRYING ANYWAY.\n";
	}
	zeroResult();
	multiplyMatrices();
	cout << "\nResult: \n\n";
	printMatrix(shared::matrix1, shared::matrix1Index);
	cout << " *\n";
	printMatrix(shared::matrix2, shared::matrix2Index);
	cout << " =\n";
	printMatrix(shared::result, shared::resultIndex);
	getch();
	return 0;
}
