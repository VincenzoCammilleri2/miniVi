#include <iostream>
#include <fstream>
#include <string>
#include "Editor.h"

using namespace std;

Editor::Editor() {

}

template<class ItemType>
void bubbleSort(ItemType theArray[], int n)
{
	bool sorted = false; // False when swaps occur
	int pass = 1;
	while (!sorted && (pass < n))
	{
		// At this point, theArray[n+1-pass..n-1] is sorted
		// and all of its entries are > the entries in theArray[0..n-pass]
		sorted = true; // Assume sorted
		for (int index = 0; index < n - pass; index++)
		{
			// At this point, all entries in theArray[0..index-1]
			// are <= theArray[index]
			int nextIndex = index + 1;
			if (theArray[index] > theArray[nextIndex])
			{
				// Exchange entries
			std:swap(theArray[index], theArray[nextIndex]);
				sorted = false; // Signal exchange
			} // end if
		}  // end for
		// Assertion: theArray[0..n-pass-1] < theArray[n-pass]

		pass++;
	}  // end while
}  // end bubbleSort

void placeCursorAt(Position coordinate) {
	COORD coord;
	coord.X = coordinate.getX();
	coord.Y = coordinate.getY();
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord);
}

void colorText(int value) {
	COORD coord;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	FlushConsoleInputBuffer(hConsole);
	SetConsoleTextAttribute(hConsole, value + 240);
}

template <typename T>
int binarySearch(const T anArray[], int first, int last, T target)
{
	int index;
	if (first > last)
		index = -1; // target not in original array
	else
	{
		// If target is in anArray, anArray[first] <= target <= anArray[last]
		int mid = first + (last - first) / 2;
		if (target == anArray[mid])
			index = mid; // target found at anArray[mid]
		else if (target < anArray[mid])
			// Point X
			index = binarySearch(anArray, first, mid - 1, target);
		else
			// Point Y
			index = binarySearch(anArray, mid + 1, last, target);
	}  // end if

	return index;
}  // end binarySearch

Editor::Editor(char* inputfile, string keywordfile) {
	ifstream inFile;
	int linePosition = 0;
	string line;
	string keyword;

	inFile.open(inputfile);

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

	inFile.open(keywordfile);

	//Check if file can be opened
	if (!inFile) {
		cout << "Error opening the file." << endl;
		exit(1);
	}

	//Iterate through the end of file
	while (!inFile.eof()) {
		inFile >> keyword;													
		keyWords[numKeywords] = keyword;					//Store each keyword
		numKeywords++;										//Increments numKeywords
	}

	bubbleSort(keyWords, MAX_SIZE);							//Sorts the keywords array
	displayLines();
}

void Editor::displayLines() {
	system("CLS");

	string nextLine;

	//Goes through each line in the linked list 
	for (int position = 1; position <= lines.getLength(); position++)
	{
		nextLine = lines.getEntry(position);

		int i = 0;
		while (i < nextLine.length()) {
			string word;
			
			//Isolate a word at a time (can contains underscores)
			if (isalpha(nextLine[i])) {
				while (isalpha(nextLine[i]) || nextLine[i] == '_') {
					word += nextLine[i];
					i++;
				}
				if (binarySearch<string>(keyWords, 0, numKeywords - 1, word) != -1)  //Found
					colorText(1);
				else
					colorText(0);
				cout << word;
			}
			else {
				colorText(0);
				cout << nextLine[i];
				i++;
			}

		}

		cout << endl;
	}

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
	lastChange.setPosition(point);
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
	lastChange.setPosition(point);
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
		exit(1);											//Quit if no changes were made in the editor
	
}

void Editor::undo() {
	//Check if the stack is empty
	if (!undoChange.isEmpty()) {
		lastChange = undoChange.peek();				//Peek at the last change saved in the stack
		point = lastChange.getPosition();			
		if (lastChange.getCommand() == 'x') {
			lines.replace(point.getY() + 1, lastChange.getChanges());		//Undo the delete character
			placeCursorAt(point);											//Moves cursor to last position
		}
		else if (lastChange.getCommand() == 'd')
		{
			lines.insert(point.getY() + 1, lastChange.getChanges());		//Undo the delete line
			placeCursorAt(point);											//Moves cursor to last position
		}
		undoChange.pop();				//Pop last change on stack
	}

	displayLines();
}