#include <iostream>
#include "Editor.h"

using namespace std;

int main(int argc, char* argv[]) {
	system("CLS");
	Editor miniVi(argv[1], "keywords.txt");					//Opens and read from file
	miniVi.run();			
	
	return 0;
}