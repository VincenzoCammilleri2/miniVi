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

private:
	LinkedList<string> lines;
	Position point;
	string filename;
	const char QUIT = 'q';
};

#include "Editor.cpp"
#endif