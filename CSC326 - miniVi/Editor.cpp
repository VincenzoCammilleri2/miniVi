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

	if (!inFile) {
		cout << "Error opening the file." << endl;
		exit(1);
	}

	while (!inFile.eof()) {
		linePosition++;
		getline(inFile, line);
		lines.insert(linePosition, line);
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
	command = _getwch();

	while (command != QUIT) {
		switch (command) {
		case 'x':													//Deletes character
			currentLine = lines.getEntry(point.getY() + 1);			//getEntry to get the line in the file
			
			lines.replace(point.getY() + 1, currentLine);			//replace function to replace the line
			
			displayLines();											//displayLines to redisplay on output screen
			
			break;
		case ':':					
			placeCursorAt(Position (endOfScreen));					//Brings cursor to bottom of the screen
			
			cout << ':';
			
			command = _getwche();									//Gets the next character enter by end-user
			
			if (command == 'w') {									//Saves file
				saveFile("TestDummy.txt");
			}
			else if (command == 'q'){								//Exits program
				exit(1);
			}
			break;
		}
	}
}

void Editor::saveFile(string filename) {
	ofstream outFile;

	outFile.open(filename, ofstream::out);

	for (int i = 1; i <= lines.getLength(); i++) {
		outFile << lines.getEntry(i) << endl;
	}

	outFile.close();
}