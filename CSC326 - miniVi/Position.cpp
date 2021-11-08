#include "Position.h"

Position::Position() {
	mX = 0;
	mY = 0;
}

Position::Position(int mX, int mY) {
	this->mX = mX;
	this->mY = mY;
}

void Position::setX(int x) {
	mX = x;
}

void Position::setY(int y) {
	mY = y;
}

const int Position::getX() {
	return mX;
}

const int Position::getY() {
	return mY;
}
