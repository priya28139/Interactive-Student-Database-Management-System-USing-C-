/********************************************************************
 File name: ass1.cpp

 Purpose: The source code file ass1.cpp provides an implementation of
 several functions, public functions listed in the app1.h header file
 as well as certain private functions, meant to accomplish the management
 of a student database system- reading, displaying, manipulating,
 updating and writing records.

 Date: 6 October 2018

 Author: Priya Aswani

 ID Number: 5925502

 Email: pa470@uowmail.edu.au

 ********************************************************************/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
using namespace std;

// ============== Constants ==========================================

const int cMaxRecs = 100;
const int cMaxChars = 30;
const int cMaxSubjects = 8;

// ============= User Defined types ==================================

enum StatusType {
	eEnrolled, eProvisional, eWithdrawn
};

struct SubjectType {
	char Code[8];
	StatusType Status;
	int Mark;
};

struct StudentRecord {
	long StudentNo;
	char FirstName[cMaxChars];
	char LastName[cMaxChars];
	int NumSubjects;
	SubjectType Subjects[cMaxSubjects];
};

// ============= Global Data =========================================

StudentRecord gRecs[cMaxRecs];
int gNumRecs = 0;
char cTextFileName[] = "ass1.txt";
char cBinaryFileName[] = "ass1.dat";

// ============= Private Function Prototypes =========================

void PrintRecord(int i);
int FindRecord(long StudentNum);
bool ReadBinaryFile(char FileName[]);
bool WriteBinaryFile(char FileName[]);
bool ReadTextFile(char FileName[]);
bool WriteTextFile(char FileName[]);
void WriteBinaryRecord(char FileName[], int Pos);

// ============= Public Function Definitions =========================

void ReadFile() {

	/*
	 * The ReadFile() function reads the contents of a binary/text file, and stores the
	 * read data, appropriately in an array of StudentRecords.
	 *
	 * The binary file specified by cBinaryFileName is attempted at being read first. In the
	 * event that it cannot be read, a text file specified by cTextFileName is tried next.
	 * Once any of them is read successfully, data is appropriately stored in the above
	 * mentioned location. If however, neither of them can be read, an error message is printed
	 * and the program terminates.
	 *
	 */

	if (!ReadBinaryFile(cBinaryFileName)) {
		if (!ReadTextFile(cTextFileName)) {
			cerr
					<< "Cannot read input binary and text files. Program terminating!"
					<< endl;
			exit(1);
		}
	}

}

void DisplayRecord() {

	/*
	 * The DisplayRecord() function displays a specific record from the array of StudentRecords
	 * and searches it based on a unique student number. In the event that the student record
	 * exists, details of the concerned record are printed, and if not an appropriate message
	 * is displayed.
	 *
	 */

	cout << "Enter student number: " << flush;
	long studentNumber;
	cin >> studentNumber;
	int i = FindRecord(studentNumber);
	if (i != -1) {
		PrintRecord(i);
	} else {
		cout << "Record not found" << endl;
	}

}

void SaveFile() {

	/*
	 * The SaveFile() function writes the contents of the StudentRecords array to a
	 * binary/text file.
	 *
	 * The binary file specified by cBinaryFileName is attempted at being written to first.
	 * In the event that it cannot be written to, a text file specified by cTextFileName is
	 * tried next.
	 *
	 * Once the program writes to any successfully, the file is saved. If however, the program
	 * fails in doing so, an appropriate error message is printed.
	 *
	 */

	if (!WriteBinaryFile(cBinaryFileName)) {
		if (!WriteTextFile(cTextFileName)) {
			cerr
					<< "Cannot write to output binary and text files. Program terminating!"
					<< endl;
			exit(1);
		}
	}

	WriteTextFile(cTextFileName);
}

void UpdateRecord() {

	/*
	 * The UpdateRecord() function updates student records based on the student number of a
	 * particular record. The record can be updated based on the subject status or mark of
	 * a particular subject listed for the record concerned.
	 *
	 */

	cout << "Enter student number: " << flush;
	long studentNumber;
	cin >> studentNumber;

	int i = FindRecord(studentNumber);
	if (i == -1) {
		cout << "Record not found!" << endl;
	} else {
		PrintRecord(i);
		cout << "Enter subject code: " << flush;

		string subjectCode;

		int subjectIndex = -1;
		cin >> subjectCode;

		for (int j = 0; j < gRecs[i].NumSubjects; j++) {
			string actualSubjectCode = gRecs[i].Subjects[j].Code;
			if (actualSubjectCode == subjectCode) {
				subjectIndex = j;
				break;
			}
		}

		if (subjectIndex == -1) {
			cout << "Subject code not found!" << endl;
		} else {
			char userSelection;
			do {
				cout << "Select status or mark (s/m): " << flush;
				cin >> userSelection;
				if (userSelection != 's' && userSelection != 'm') {
					cout << "Invalid selection!" << endl;
				}
			} while (userSelection != 's' && userSelection != 'm');

			if (userSelection == 's') {

				char statusSelection;
				do {
					cout << "Select new status" << endl;

					cout << "\te:enrolled" << endl;
					cout << "\tp:provisional" << endl;
					cout << "\tw:withdrawn" << endl;

					cin >> statusSelection;
					if (statusSelection != 'e' && statusSelection != 'p'
							&& statusSelection != 'w') {
						cout << "Invalid Selection!" << endl;
					}
				} while (statusSelection != 'e' && statusSelection != 'p'
						&& statusSelection != 'w');

				switch (statusSelection) {
				case 'e':
					gRecs[i].Subjects[subjectIndex].Status = eEnrolled;
					break;
				case 'p':
					gRecs[i].Subjects[subjectIndex].Status = eProvisional;
					break;
				case 'w':
					gRecs[i].Subjects[subjectIndex].Status = eWithdrawn;
					break;

				}
			}

			else {
				int mark;

				do {
					cout << "Enter new mark: " << flush;
					cin >> mark;
					if (mark < 0 || mark > 100) {
						cout << "Invalid Entry!" << endl;
					}
				} while (mark < 0 || mark > 100);

				gRecs[i].Subjects[subjectIndex].Mark = mark;

			}

			WriteBinaryRecord(cBinaryFileName, i);

			cout << "Record " << studentNumber << " Updated." << endl;

		}

	}
}

void AddRecord() {

	/*
	 * The AddRecord() function enables the addition of a new student record to the
	 * database, provided it has not reached its upper limit.
	 *
	 */

	if (gNumRecs == cMaxRecs) {
		cout << "Database full! Record cannot be added." << endl;
	} else {

		int currentStudentIndex = gNumRecs;
		cout << "Enter new student details as directed below." << endl;
		cout << "Student Number: " << flush;
		long studentNumber;
		cin >> studentNumber;
		if (FindRecord(studentNumber) != -1) {
			cout << "Student Record Already Exists!" << endl;
			return;
		}
		gRecs[currentStudentIndex].StudentNo = studentNumber;
		cout << "First Name: " << flush;
		cin >> gRecs[currentStudentIndex].FirstName;
		cout << "Last Name: " << flush;
		cin >> gRecs[currentStudentIndex].LastName;
		do {
			cout << "Number of Subjects: " << flush;
			cin >> gRecs[currentStudentIndex].NumSubjects;
			if (gRecs[currentStudentIndex].NumSubjects > 8) {
				cout
						<< "Invalid entry! Maximum number of subjects that can be taken is 8"
						<< endl;
			}
		} while (gRecs[currentStudentIndex].NumSubjects > 8);

		if(gRecs[currentStudentIndex].NumSubjects!=0){
			cout << "Enter subject details as directed below." << endl;
		}

		for (int i = 0; i < gRecs[currentStudentIndex].NumSubjects; i++) {
			cout << "Subject#" << (i + 1) << ":" << endl;
			cout << "Subject Code: " << flush;
			cin >> gRecs[currentStudentIndex].Subjects[i].Code;

			char subjectStatus;
			do {
				cout
						<< "Subject Status:\n\te:enrolled\n\tp:provisional\n\tw:withdrawn "
						<< flush;

				cin >> subjectStatus;
				if (subjectStatus != 'e' && subjectStatus != 'p'
						&& subjectStatus != 'w') {
					cout << "Invalid entry!" << endl;
				}
			} while (subjectStatus != 'e' && subjectStatus != 'p'
					&& subjectStatus != 'w');

			switch (subjectStatus) {
			case 'e':
				gRecs[currentStudentIndex].Subjects[i].Status = eEnrolled;
				break;
			case 'p':
				gRecs[currentStudentIndex].Subjects[i].Status = eProvisional;
				break;
			case 'w':
				gRecs[currentStudentIndex].Subjects[i].Status = eWithdrawn;
				break;

			}

			int mark;
			do {
				cout << "Subject Mark: " << flush;
				cin >> mark;
				if (mark < 0 || mark > 100) {
					cout << "Invalid entry!" << endl;
				}
			} while (mark < 0 || mark > 100);

			gRecs[currentStudentIndex].Subjects[i].Mark = mark;

			cout << endl;

		}

		gNumRecs++;

	}
}

// ============= Private Function Definitions =========================

void PrintRecord(int i) {

	/*
	 * The PrintRecord() function prints a specific student record specified by an index
	 * in the StudentRecords array.
	 *
	 */

	cout << left;
	cout << setw(15) << "Student No." << gRecs[i].StudentNo << endl;
	cout << setw(15) << "First Name" << gRecs[i].FirstName << endl;
	cout << setw(15) << "Last Name" << gRecs[i].LastName << endl;
	cout << "Subjects: " << endl;

	for (int j = 0; j < gRecs[i].NumSubjects; j++) {
		cout << "\t" << gRecs[i].Subjects[j].Code << "  " << flush;
		if (gRecs[i].Subjects[j].Status == eEnrolled) {
			cout << "Enrolled  " << flush;
		} else if (gRecs[i].Subjects[j].Status == eProvisional) {
			cout << "Provisional  " << flush;
		} else {
			cout << "Withdrawn  " << flush;
		}

		cout << gRecs[i].Subjects[j].Mark << endl;
	}

	cout << endl << endl;

}

int FindRecord(long StudentNo) {

	/*
	 * The FindRecord() function searches the StudentRecords array to determine if a particular
	 * student record exists or otherwise. In case the record exists, the function returns
	 * its position in the array. If not, the function returns -1, to indicate that the
	 * said record is not in existence.
	 *
	 */

	for (int i = 0; i < gNumRecs; i++) {
		if (gRecs[i].StudentNo == StudentNo) {
			return i;
		}
	}

	return -1;
}

bool ReadTextFile(char FileName[]) {

	/*
	 * The ReadTextFile() function attempts reading a text file, whose name is specified by
	 * the formal parameter FileName. Once the file is read successfully, data is appropriately
	 * stored in the StudentRecords array and the function returns true to the calling
	 * environment. If not, false is returned to the caller.
	 *
	 */

	ifstream inFile;
	inFile.open(FileName);

	if (!inFile.good()) {
		return false;
	}

	int i = 0;
	int subjectCode;
	string blankLine;

	while (inFile.peek() != EOF) {

		inFile >> gRecs[i].StudentNo;
		inFile >> gRecs[i].FirstName;
		inFile >> gRecs[i].LastName;
		inFile >> gRecs[i].NumSubjects;

		for (int j = 0; j < gRecs[i].NumSubjects; j++) {
			inFile >> gRecs[i].Subjects[j].Code;
			inFile >> subjectCode;
			if (subjectCode == 0) {
				gRecs[i].Subjects[j].Status = eEnrolled;
			} else if (subjectCode == 1) {
				gRecs[i].Subjects[j].Status = eProvisional;
			} else {
				gRecs[i].Subjects[j].Status = eWithdrawn;
			}
			inFile >> gRecs[i].Subjects[j].Mark;
		}

		getline(inFile, blankLine);
		i++;

	}

	gNumRecs = i;
	inFile.close();
	cout << gNumRecs << " records read" << endl;

	return true;
}

bool WriteTextFile(char FileName[]) {

	/*
	 * The WriteTextFile() function attempts writing to a text file, whose name is specified by
	 * the formal parameter FileName. Once the file is written to successfully the function
	 * returns true to the calling environment. And if not, false is returned to its caller.
	 *
	 */

	ofstream outFile;
	outFile.open(FileName);

	if (!outFile.good()) {
		return false;
	}

	for (int i = 0; i < gNumRecs; i++) {

		outFile << gRecs[i].StudentNo << endl;
		outFile << gRecs[i].FirstName << " " << gRecs[i].LastName << endl;
		outFile << gRecs[i].NumSubjects << endl;

		for (int j = 0; j < gRecs[i].NumSubjects; j++) {
			outFile << gRecs[i].Subjects[j].Code << " "
					<< gRecs[i].Subjects[j].Status << " "
					<< gRecs[i].Subjects[j].Mark << endl;
		}

		if (i != gNumRecs - 1) {
			outFile << endl;
		}

	}

	outFile.close();
	cout << gNumRecs << " records saved" << endl;
	return true;
}

bool ReadBinaryFile(char FileName[]) {

	/*
	 * The ReadTextFile() function attempts reading a binary file, whose name is specified by
	 * the formal parameter FileName. Once the file is read successfully, data is appropriately
	 * stored in the StudentRecords array and the function returns true to the calling
	 * environment. If not, false is returned to the caller.
	 *
	 */

	ifstream inFile;
	inFile.open(FileName, ios::binary);

	if (!inFile.good()) {
		return false;
	}

	StudentRecord temp;
	inFile.read(reinterpret_cast<char*>(&gNumRecs), sizeof(gNumRecs));

	for (int i = 0; i < gNumRecs; i++) {

		inFile.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		gRecs[i].StudentNo = temp.StudentNo;
		strcpy(gRecs[i].FirstName, temp.FirstName);
		strcpy(gRecs[i].LastName, temp.LastName);

		gRecs[i].NumSubjects = temp.NumSubjects;

		for (int j = 0; j < gRecs[i].NumSubjects; j++) {
			strcpy(gRecs[i].Subjects[j].Code, temp.Subjects[j].Code);
			gRecs[i].Subjects[j].Status = temp.Subjects[j].Status;
			gRecs[i].Subjects[j].Mark = temp.Subjects[j].Mark;
		}

	}

	inFile.close();
	cout << gNumRecs << " records read" << endl;
	return true;
}

bool WriteBinaryFile(char FileName[]) {

	/*
	 * The WriteBinaryFile() function attempts writing to a binary file, whose name is specified
	 * by the formal parameter FileName. Once the file is written to successfully the function
	 * returns true to the calling environment. And if not, false is returned to its caller.
	 *
	 */

	ofstream outFile;
	outFile.open(FileName, ios::binary);

	if (!outFile.good()) {
		return false;
	}

	outFile.write(reinterpret_cast<const char*>(&gNumRecs), sizeof(gNumRecs));

	StudentRecord temp;

	for (int i = 0; i < gNumRecs; i++) {
		temp = gRecs[i];
		outFile.write(reinterpret_cast<const char*>(&temp), sizeof(temp));
	}

	outFile.close();
	cout << gNumRecs << " records saved" << endl;
	return true;
}

void WriteBinaryRecord(char FileName[], int Pos) {

	/*
	 * The WriteBinaryRecord() function writes a specific StudentRecord to the binary file
	 * FileName at the position specified by Pos. The sub-routine makes use of the built in
	 * function seekp() to randomly access the file contents and manipulate them in place.
	 *
	 */

	ofstream outFile;
	outFile.open(FileName, ios::binary);

	if (!outFile.good()) {
		cerr << "Cannot write to binary file!" << endl;
		exit(1);
	}

	StudentRecord student = gRecs[Pos];
	long studentRecordSize = sizeof(student);

	outFile.seekp((Pos - 1) * studentRecordSize, ios::beg);
	outFile.write(reinterpret_cast<const char*>(&student), sizeof(student));

	outFile.close();

}
