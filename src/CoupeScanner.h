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
			Scanner() : position(1, 0), 
						verbose(false),	
						inputStream(&std::cin),
						outputStream(&std::cout) {}	

			Token* getNext();			
			void setInputFile(std::string fileName);
			void setInputStream(std::istream& stream);
			void setOutputStream(std::istream& stream);
			void beVerbose(bool _verbose);

		private:
			Position position;						
			bool verbose;
			std::istream* inputStream;
			std::ostream* outputStream;

			Token* getNextToken();
			Token* createToken(Type type, std::string value, Position position);			
			void beVerboseAboutToken(Token* token);
			char getCharacter();
			void ungetCharacter();
			void toNextRow();
	};
}

#endif