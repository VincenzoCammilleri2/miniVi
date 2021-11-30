#pragma once

#ifndef _EDITOR
#define _EDITOR

#include "LinkedList.h"
#include "LinkedStack.h"
#include "Position.h"
#include "Windows.h"
#include "Changes.h"

using namespace std;

class Editor {
public:
	Editor();
	Editor(string filename);
	void displayLines();
	void run();
	void deleteCharacter();
	void saveFile(string filename);
	void commandMode();
	void deleteLine();
	void moveDown();
	void moveUp();
	void moveLeft();
	void moveRight();
	void quit();
	void undo();

private:
	LinkedList<string> lines;
	LinkedStack<Changes> undoChange;
	Position point;
	Changes lastChange;
	bool changes = false;
};

#endif