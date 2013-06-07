#include <iostream>
#include <string>

#include "CoupeScanner.h"
#include "CoupeUtils.h"
#include "CoupeToken.h"

int main()
{
	std::string filename = "test_files/coupe1.txt";

	Coupe::Scanner::getInstance().setFile(filename);
	//scanner.setStream(std::cin);

	Coupe::Token* token;
	while((token = Coupe::Scanner::getInstance().getNext())->type != Coupe::TOK_EOF) {
		std::cout << Coupe::Utils::createTokenInfo(token) << std::endl;
	}
	std::cout << Coupe::Utils::createTokenInfo(token) << std::endl;
	getchar();
}