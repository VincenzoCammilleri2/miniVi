#include <iostream>
#include <string>
#include "Changes.h"
#include "Position.h"

Changes::Changes() {

}

void Changes::setChanges(string userChanges) {
	this->userChanges = userChanges;
}

void Changes::setPosition(Position userPosition) {
	this->userPosition = userPosition;
}

void Changes::setCommand(char userCommand) {
	this->userCommand = userCommand;
}

string Changes::getChanges() {
	return userChanges;
}

Position Changes::getPosition() {
	return userPosition;
}

char Changes::getCommand() {
	return userCommand;
}