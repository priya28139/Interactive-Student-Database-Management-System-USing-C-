/********************************************************************
 File name: main.cpp

 Purpose: The source code file main.cpp provides an environment for
 the actual utilisation of the sub-routines implemented in the ass1.cpp
 aimed at managing a student database system.

 Date: 6 October 2018

 Author: Priya Aswani

 ID Number: 5925502

 Email: pa470@uowmail.edu.au

 ********************************************************************/

#include <iostream>
#include <cctype>
#include "ass1.h"
using namespace std;

// ============= Function Prototypes =========================
void PrintMenu();

int main() {

	char Command;
	ReadFile();
	PrintMenu();
	do {
		cout << "Command: ";
		cin >> Command;
		Command = tolower(Command);
		switch (Command) {
		case 'u':
			UpdateRecord();
			break;
		case 'd':
			DisplayRecord();
			break;
		case 'm':
			PrintMenu();
			break;
		case 'a':
			AddRecord();
			break;
		case 'q':
			break;

		default:
			cerr << "Illegal Command\n";
			PrintMenu();
		}
	} while (Command != 'q');

	SaveFile();

	return 0;
}

void PrintMenu() {

	/*
	 * Displays a menu from which the user can make a desired selection of displaying a record,
	 * updating a record, printing the menu or quitting the program
	 *
	 */
	cout << "Commands Available:\n";
	cout << "  d - Display Record\n";
	cout << "  u - Update Record\n";
	cout << "  m - Print Menu\n";
	cout << "  a - Add Record\n";
	cout << "  q - Quit the program\n\n";
}
