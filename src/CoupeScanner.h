#ifndef COUPE_SCANNER_H_
#define COUPE_SCANNER_H_

#include "CoupeToken.h"

#include <fstream>

namespace Coupe
{
	class Scanner
	{
		public:			
			void setFile(std::string _filename);
			void setStream(std::istream& stream);
			Token* getNext();

			static Scanner& getInstance() 
			{
				static Scanner instance;
				return instance;
			}
		private:
			Scanner() {}						
			Scanner(const Scanner&) {}
			Scanner& operator=(const Scanner&) {}

			Position currentPosition;
			std::ifstream currentFile;
			std::istream* currentStream;

			Token* createToken(Type type, std::string value, Position position);
	};
}

#endif