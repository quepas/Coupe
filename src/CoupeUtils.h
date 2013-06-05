#ifndef COUPE_UTILS_H_
#define COUPE_UTILS_H_

#include "CoupeToken.h"

#include <sstream>

namespace Coupe
{
	template<typename T_out, typename T_in>
	T_out lexical_cast(T_in inVal)
	{
		T_out outVal;
		std::stringstream lexicalStream;

		lexicalStream << inVal;
		lexicalStream >> outVal;
		return outVal;
	}

	class Utils
	{
		public:
			static bool isWhitespace(char character);
			static bool isDigit(char character);
			static bool isCharacter(char character);

			// temporary
			static std::string createTokenInfo(Token* token);
		private:
			Utils() {}
			Utils(const Utils&) {}
			Utils& operator=(const Utils&) {}
	};
}

#endif