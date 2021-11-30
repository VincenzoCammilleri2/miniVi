#include <iostream>
#include <string>
#include "Changes.h"
#include "Position.h"

Changes::Changes() {

}

void Changes::setChanges(string userChanges) {
	this->userChanges = userChanges;
}

void Changes::setPosition(int userPosition) {
	this->userPosition = userPosition;
}

void Changes::setCommand(char userCommand) {
	this->userCommand = userCommand;
}

string Changes::getChanges() {
	return userChanges;
}

int Changes::getPosition() {
	return userPosition;
}

char Changes::getCommand() {
	return userCommand;
}