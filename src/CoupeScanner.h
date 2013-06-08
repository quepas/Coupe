#ifndef COUPE_SCANNER_H_
#define COUPE_SCANNER_H_

#include "CoupeToken.h"

#include <fstream>

namespace Coupe
{
	class Scanner
	{
		public:			
			Scanner() : currentPosition(1, 0), currentStream(nullptr) {}	

			void setFile(std::string _filename);
			void setStream(std::istream& stream);
			Token* getNext();			
		private:
			Position currentPosition;			
			std::istream* currentStream;

			Token* createToken(Type type, std::string value, Position position);
	};
}

#endif