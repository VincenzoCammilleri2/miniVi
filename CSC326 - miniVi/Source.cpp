#include <iostream>
#include "Editor.h"

using namespace std;

int main() {
	Editor miniVi("TestDummy.txt");
	system("CLS");
	miniVi.displayLines();
}