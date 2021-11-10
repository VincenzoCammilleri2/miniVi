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
	changes = false;

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

	while ((command = _getwch())) {
		switch (command) {
		case 'x':													//Deletes character
			deleteCharacter();
			break;
		case ':':													//Save or Quit
			commandMode();											
			break;
		case 'd':													//Delete current line
			deleteLine();
			break;
		case 'j':													//Move cursor down 
			moveDown();
			break;
		case 'k':													//Move cursor up
			moveUp();
			break;
		case 'h':													//Move cursor left
			moveLeft();
			break;
		case 'l':													//Move cursor right
			moveRight();
			break;
		case 'q':													//Quit without any changes
			quit();
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
	
	changes = true;											//Set changes to file as true
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

void Editor::deleteLine() {
	char command;

	cout << 'd';

	command = _getwche();									//Gets the next character enter by end-user

	if (command == 'd')										//Removes the current line
		lines.remove(point.getY() + 1);

	changes = true;											//Set changes to file as true
	displayLines();											//displayLines to redisplay on output screen
}

void Editor::moveDown() {
	int down = point.getY();

	//Moves cursor down
	if (point.getY() < lines.getLength()) 
		point.setY(down + 1); 

	placeCursorAt(point);
}

void Editor::moveUp() {
	int up = point.getY();

	//Moves cursor up
	if (point.getY() > 0) 
		point.setY(up - 1);

	placeCursorAt(point);
}

void Editor::moveLeft() {
	int left = point.getX();

	//Allow cursor to move left
	if (point.getX() >= 0) 
		point.setX(left - 1);
	else 
		point.setX(0);				//Resets the cursor if out of bounds

	placeCursorAt(point);
}

void Editor::moveRight() {
	int right = point.getX();

	//Allows cursor to move right
	if (point.getX() >= 0) 
		point.setX(right + 1);

	placeCursorAt(point);
}

void Editor::quit() {
	char command;
	int bottom = lines.getLength() + 1;
	Position endOfScreen(0, bottom);
	char answer;

	placeCursorAt(Position(endOfScreen));					//Brings cursor to bottom of the screen

	cout << 'q';

	command = _getwche();									//Gets the next character enter by end-user

	if (command == '!') {
		if (changes == true) {
			cout << endl << "Are you sure you want to quit? (y/n): " << endl;			//Prompts the end-user about quitting with changes made
			cin >> answer;

			if (answer == 'y' || answer == 'Y')				//Will exit the program
				exit(1);
			else 											//Will run the editor
				run();
		}
		
		exit(1);						//Quit if no changes were made in the editor
	}
}