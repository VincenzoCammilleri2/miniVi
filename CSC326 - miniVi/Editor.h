#pragma once

#ifndef _EDITOR
#define _EDITOR

#include "LinkedList.h"
#include "LinkedStack.h"
#include "Position.h"
#include "Windows.h"

using namespace std;

class Editor {
public:
	Editor();
	Editor(string filename);
	void displayLines();
	void run();
	void deleteCharacter();
	void saveFile(string filename);

private:
	LinkedList<string> lines;
	Position point;
	string filename;
	char deleteChar;
	const char QUIT = 'q';

};

#include "Editor.cpp"
#endif