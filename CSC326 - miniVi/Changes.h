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
	void setPosition(int);
	void setCommand(char);
	string getChanges();
	int getPosition();
	char getCommand();

private:
	string userChanges;
	int userPosition;
	char userCommand;
};