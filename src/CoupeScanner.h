#ifndef COUPE_SCANNER_H_
#define COUPE_SCANNER_H_

#include "CoupeToken.h"

#include <fstream>

namespace Coupe
{
	class Scanner
	{
		public:
			Scanner() {}
			~Scanner() {}
			
			void setFile(std::string _filename);
			Token* getNext();
		private:
			Position currentPosition;
			std::ifstream currentFile;

			Token* createToken(Type type, std::string value, Position position);
	};
}

#endif