#pragma once

#include <iostream>
#include <string>
#include "Position.h"

using namespace std;

class Changes
{
public:
	Changes();
	void setChanges(string);
	void setPosition(Position);
	void setCommand(char);
	string getChanges();
	Position getPosition();
	char getCommand();

private:
	string userChanges;
	Position userPosition;
	char userCommand;
};