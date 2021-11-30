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
		case 'q':													//Quit 
			quit();
			break;
		case 'u':													//Undo last change
			undo();
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

void Editor::deleteCharacter() {							//works VC
	string currentLine;
	
	currentLine = lines.getEntry(point.getY() + 1);			//Stores copy of string using getEntry to get the line in the file

	lastChange.setChanges(currentLine);
	lastChange.setPosition(point.getY() + 1);
	lastChange.setCommand('x');

	undoChange.push(lastChange);

	currentLine.erase(point.getX(), 1);						//Deletes the character at cursor position (x)

	lines.replace(point.getY() + 1, currentLine);			//replace function to replace the line

	displayLines();											//displayLines to redisplay on output screen
	
	changes = true;											//Set changes to file as true
}

void Editor::commandMode() {								
	char command;
	char answer;
	int bottom = lines.getLength();
	Position endOfScreen(0, bottom);				

	placeCursorAt(Position(endOfScreen));					//Brings cursor to bottom of the screen

	cout << ':';

	command = _getwche();									//Gets the next character enter by end-user

	if (command == 'w') {									//Saves file
		saveFile("TestDummy.txt");	
		changes = false;
	}
	else if (command == 'q') {								//Exits program
		if (changes == true) {
			cout << endl << "Are you sure you want to quit? (y/n): " << endl;			//Prompts the end-user about quitting with changes made
			cin >> answer;

			if (answer == 'y' || answer == 'Y')				//Will exit the program
				exit(1);
			else											//Will run the editor
				displayLines();

		}
		else
			exit(1);
	}
}

void Editor::deleteLine() { 
	string currentLine = lines.getEntry(point.getY() + 1);
	char command;

	command = _getwch();									//Gets the next character enter by end-user

	lastChange.setChanges(currentLine);
	lastChange.setPosition(point.getY() + 1);
	lastChange.setCommand('d');

	undoChange.push(lastChange);

	if (command == 'd')										//Removes the current line
		lines.remove(point.getY() + 1);

	changes = true;											//Set changes to file as true
	displayLines();											//displayLines to redisplay on output screen
}

void Editor::moveDown() {
	if (point.getY() == lines.getLength() - 1)				//Returns if you try to go down at the end of the list
		return;

	int down = point.getY();
	string nextLine = lines.getEntry(point.getY() + 2);		

	//Moves cursor down
	if (point.getX() >= nextLine.length()) {				//Checks if cursor is at the end of the string
		point.setY(down + 1);
		point.setX(nextLine.length() - 1);
	}
	else {
		point.setY(down + 1);								//Moves down if cursor is not at the end of the string
	}

	placeCursorAt(point);
}

void Editor::moveUp() {
	if (point.getY() == 0)									//Returns if you try to go up at the beginning of the list
		return;

	int up = point.getY();
	string prevLine = lines.getEntry(point.getY());

	//Moves cursor up
	if (point.getX() >= prevLine.length()) {				//Checks if the cursor is at the end of the string
		point.setY(up - 1);
		point.setX(prevLine.length() - 1);
	}
	else {
		point.setY(up - 1);									//Moves up if cursor is not at the end of the string
	}

	placeCursorAt(point);
}

void Editor::moveLeft() {
	int left = point.getX();

	////Allow cursor to move left
	if (left > 0) 
		point.setX(left - 1);
	
	placeCursorAt(point);
}

void Editor::moveRight() {
	int right = point.getX();

	//Allows cursor to move right to the end of the string
	if (right >= 0 && right < lines.getEntry(point.getY() + 1).length() - 1) 
		point.setX(right + 1);

	placeCursorAt(point);
}

void Editor::quit() {
	char command;
	int bottom = lines.getLength();
	Position endOfScreen(0, bottom);
	char answer;

	placeCursorAt(Position(endOfScreen));					//Brings cursor to bottom of the screen

	cout << 'q';

	command = _getwche();									//Gets the next character enter by end-user

	if (command == '!') 
		exit(1);						//Quit if no changes were made in the editor
	
}

void Editor::undo() {
	if (!undoChange.isEmpty()) {
		lastChange = undoChange.peek();
		if (lastChange.getCommand() == 'x') {
			lines.replace(lastChange.getPosition(), lastChange.getChanges());
		}
		else if (lastChange.getCommand() == 'd')
		{
			lines.insert(lastChange.getPosition(), lastChange.getChanges());
		}
		undoChange.pop();
	}

	displayLines();
}