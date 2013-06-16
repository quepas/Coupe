#include <iostream>
#include <string>

#include "CoupeScanner.h"
#include "CoupeParser.h"
#include "CoupeToken.h"

int main()
{
	std::string fileName = "test_files/coupe1.txt";
	std::string fileNameFib = "test_files/coupe_fib.txt";

	Coupe::Parser parser;
	parser.beVerbose(true);
	//parser.setInputFile(fileName);
	// tu bylem: mpanek
	parser.parse();

	getchar();
}