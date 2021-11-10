#include <iostream>
#include <fstream>
#include "Editor.h"

using namespace std;

Editor::Editor() {

}

Editor::Editor(string filename) {
	ifstream inFile;
	int linePosition = 0;
	string line;

	inFile.open(filename);

	//Check if file can be opened
	if (!inFile) {
		cout << "Error opening the file." << endl;
		exit(1);
	}

	//Iterate through the end of file
	while (!inFile.eof()) {
		linePosition++;										//Increases line position
		getline(inFile, line);								//Gets the whole line
		lines.insert(linePosition, line);					//Stores each line in a node
	}

	inFile.close();

	displayLines();
}

void placeCursorAt(Position coordinate) {			
	COORD coord;
	coord.X = coordinate.getX();
	coord.Y = coordinate.getY();
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord);
}

void Editor::displayLines() {
	system("CLS");

	//Prints every line from the linked list
	for (int position = 1; position <= lines.getLength(); position++) {	
		cout << lines.getEntry(position) << endl;
	}

	//Places cursor at coordinates (0, 0)
	placeCursorAt(point);
}

void Editor::run() {
	char command;
	string currentLine;
	int bottom = lines.getLength() + 1;
	Position endOfScreen(0, bottom);

	while ((command = _getwch()) != QUIT) {
		switch (command) {
		case 'x':													//Deletes character
			deleteCharacter();
			break;
		case ':':					
			commandMode();											//Save or Quit
			break;
		}
	}
}

void Editor::saveFile(string filename) {
	ofstream outFile;

	outFile.open(filename, ofstream::out);

	for (int i = 1; i <= lines.getLength(); i++) {					//Saves lines in outFile
		outFile << lines.getEntry(i) << endl;
	}

	outFile.close();
}

void Editor::deleteCharacter() {
	string currentLine;
	
	currentLine = lines.getEntry(point.getY() + 1);			//Stores copy of string using getEntry to get the line in the file

	currentLine.erase(point.getX(), 1);						//Deletes the character at cursor position (x)

	lines.replace(point.getY() + 1, currentLine);			//replace function to replace the line

	displayLines();											//displayLines to redisplay on output screen
}

void Editor::commandMode() {
	char command;
	int bottom = lines.getLength() + 1;
	Position endOfScreen(0, bottom);				

	placeCursorAt(Position(endOfScreen));					//Brings cursor to bottom of the screen

	cout << ':';

	command = _getwche();									//Gets the next character enter by end-user

	if (command == 'w') {									//Saves file
		saveFile("TestDummy.txt");
	}
	else if (command == 'q') {								//Exits program
		exit(1);
	}
}