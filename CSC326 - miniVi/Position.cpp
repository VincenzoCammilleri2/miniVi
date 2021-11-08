#include "Position.h"

Position::Position() {
	
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
