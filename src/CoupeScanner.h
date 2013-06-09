#ifndef COUPE_SCANNER_H_
#define COUPE_SCANNER_H_

#include "CoupeToken.h"

#include <iostream>
#include <fstream>

namespace Coupe
{
	class Scanner
	{
		public:			
			Scanner() : inputStream(&std::cin),
						outputStream(&std::cout),
						currentPosition(1, 0), 
						verbose(false) {}	

			void setInputFile(std::string _filename);
			void setInputStream(std::istream& _stream);
			void setOutputStream(std::istream& _stream);
			void setVerbose(bool _verbose);
			Token* getNext();			
		private:
			Position currentPosition;			
			std::istream* inputStream;
			std::ostream* outputStream;
			bool verbose;

			Token* getNextToken();
			Token* createToken(Type type, std::string value, Position position);
			void beVerboseAboutToken(Token* token);
	};
}

#endif