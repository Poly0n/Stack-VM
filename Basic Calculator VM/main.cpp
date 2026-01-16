#include "stack.h"
#include <cstring>
#include <fstream>
#include <iostream>

int main()
{
	stack stck;

	std::filesystem::path cwd = std::filesystem::current_path();
	std::filesystem::path filePath = cwd / "example.txt";
	std::string file = filePath.string();

	if (stck.loadFromFile(file)) {
		stck.printInstructions();
		stck.execute();
	} else {
		std::cerr << "\n[!] File error: check if dataset is too large or file path is wrong.";
	}

	getchar();

	return 0;
}
