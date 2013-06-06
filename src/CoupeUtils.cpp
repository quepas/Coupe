#include "CoupeUtils.h"

#include <algorithm>

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

	// TODO: modify for UTF-8
	std::string Utils::toUpperCase(std::string text)
	{
		std::string newText;
		std::for_each(text.begin(), text.end(), [&newText](char character){
			if(isCharacter(character) || isDigit(character))
			{
				if(character >= 'a' && character <= 'z')
				{
					newText.push_back((int)character - 32);
				}
				else
				{
					newText.push_back(character);
				}
			} 		
		});
		return newText;
	}

	// TODO: modify for UTF-8
	std::string Utils::toLowerCase(std::string text)
	{
		std::string newText;
		std::for_each(text.begin(), text.end(), [&newText](char character){
			if(isCharacter(character) || isDigit(character))
			{
				if(character >= 'A' && character <= 'Z')
				{
					newText.push_back((int)character + 32);
				}
				else
				{
					newText.push_back(character);
				}
			} 		
		});
		return newText;
	}

	// temporary
	std::string Utils::createTokenInfo(Token* token) 
	{
		std::string result = "";

		result.append(getTokenName(token -> type) + " ");
		result.append("(\'" + token -> value.value + "\'");
		result.append(" [" + lexical_cast<std::string>(token -> position.row) + 
					  ", " + lexical_cast<std::string>(token -> position.col) + "])");

		return result;
	}

	std::string Utils::getTokenName(Type type)
	{
		switch(type)
		{
			CASE_TYPE(TOK_EOF)
			CASE_TYPE(TOK_ERROR)
			CASE_TYPE(TOK_IDENTIFIER)
			CASE_TYPE(TOK_KW_DEF)
			CASE_TYPE(TOK_KW_EXTERN)
			CASE_TYPE(TOK_KW_BEGIN)
			CASE_TYPE(TOK_KW_END)
			CASE_TYPE(TOK_KW_IF)
			CASE_TYPE(TOK_KW_ELSE)
			CASE_TYPE(TOK_KW_LOOP)
			CASE_TYPE(TOK_KW_RETURN)
			CASE_TYPE(TOK_KW_AND)
			CASE_TYPE(TOK_KW_OR)
			CASE_TYPE(TOK_NUMBER)
			CASE_TYPE(TOK_ROUND_LEFT_BRACKET)
			CASE_TYPE(TOK_ROUND_RIGHT_BRACKET)
			CASE_TYPE(TOK_OP_IMPLICATION)
			CASE_TYPE(TOK_OP_MUL)
			CASE_TYPE(TOK_OP_DIV)
			CASE_TYPE(TOK_OP_MOD)
			CASE_TYPE(TOK_OP_SUB)
			CASE_TYPE(TOK_OP_ADD)
			CASE_TYPE(TOK_OP_POWER)
			CASE_TYPE(TOK_COMMENT_LINE)
			CASE_TYPE(TOK_COMMENT_SECTION)
		}
	}
}