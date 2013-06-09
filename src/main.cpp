#include <iostream>
#include <string>

#include "CoupeScanner.h"
#include "CoupeParser.h"
#include "CoupeUtils.h"
#include "CoupeToken.h"

int main()
{
	std::string filename = "test_files/coupe1.txt";

	Coupe::Parser parser;
	parser.setVerbose(true);
	parser.setInputFile(filename);
	parser.parse();

	std::cout << "===== Scanner once again (just for fun and tests) =====" << std::endl;
	Coupe::Scanner scanner;
	//scanner.setVerbose(true);
	scanner.setInputFile(filename);
	//scanner.setStream(std::cin);

	Coupe::Token* token;
	while((token = scanner.getNext())->type != Coupe::TOK_EOF) {}	
	getchar();
}