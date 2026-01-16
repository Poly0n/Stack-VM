#include "stack.h"
#include <cstring>
#include <fstream>
#include <iostream>

int main()
{
	stack stck;

	std::string file = "C:\\whatever.txt";

	if (stck.loadFromFile(file)) {
		stck.printInstructions();
		stck.execute();
	} else {
		std::cerr << "\n[!] File error: check if dataset is too large or file path is wrong.";
	}

	getchar();

	return 0;
}