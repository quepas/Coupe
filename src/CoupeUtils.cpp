#include "CoupeUtils.h"

namespace Coupe
{
	// TODO: modify for UTF-8
	bool Utils::isWhitespace(char character)
	{
		if(character == ' ' || character == '\t' || character == '\n') {
			return true;
		}
		return false;
	}

	// TODO: modify for UTF-8
	bool Utils::isDigit(char character)
	{
		if(character >= '0' && character <= '9') {
			return true;
		}
		return false;
	}

	// TODO: modify for UTF-8
	bool Utils::isCharacter(char character) 
	{
		if((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z')) {
			return true;
		}
		return false;
	}

	std::string Utils::createTokenInfo(Token* token) 
	{
		std::string result = "";

		result.append("\'" + token -> value.value + "\'");
		result.append(" [" + lexical_cast<std::string>(token -> position.row) + 
					  ", " + lexical_cast<std::string>(token -> position.col) + "]");

		return result;
	}
}