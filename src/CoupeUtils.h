#ifndef COUPE_UTILS_H_
#define COUPE_UTILS_H_

#include "CoupeToken.h"

#include <sstream>

#define TOKEN_TYPE_TO_STRING(type) std::string(#type)
#define CASE_TYPE(type)	\
	case type:			\
	return TOKEN_TYPE_TO_STRING(type);

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
			static std::string toUpperCase(std::string text);
			static std::string toLowerCase(std::string text);
			static char* createCString(std::string text);

			// temporary
			static std::string createTokenInfo(Token* token);
			static std::string getTokenName(Type type);
		private:
			Utils() {}
			Utils(const Utils&) {}
			Utils& operator=(const Utils&) {}
	};
}

#endif