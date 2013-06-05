#include "CoupeScanner.h"

#include "CoupeUtils.h"
#include <iostream>

namespace Coupe
{
	// TODO: rewrite - use base istream!
	void Scanner::setFile(std::string _filename)
	{
		currentFile.close();
		currentFile.open(_filename);
		if(!currentFile.is_open()) 
		{
			std::cout << "Cannot open file: " << _filename << "!" << std::endl;
		}
		currentPosition.set(1, 0);
	}

	Token* Scanner::getNext()
	{					
		char currentChar;
		std::string currentValue = "";
		Position tokenPosition;
		// get next chars and remove all whitespaces
		do 
		{
			currentChar = currentFile.get();
			++currentPosition.col;

			if(currentChar == '\n')
			{
				++currentPosition.row;
				currentPosition.col = 0;
			}
		} while (Utils::isWhitespace(currentChar));
		tokenPosition.set(currentPosition.row, currentPosition.col);

		// identifier or keyword
		if(Utils::isCharacter(currentChar)) 
		{
			do 
			{
				currentValue.push_back(currentChar);
				currentChar = currentFile.get();
				++currentPosition.col;
			} while (Utils::isCharacter(currentChar));
			currentFile.unget();
			--currentPosition.col;

			if(currentValue == "def") return createToken(TOK_KW_DEF, currentValue, tokenPosition);
			if(currentValue == "extern") return createToken(TOK_KW_EXTERN, currentValue, tokenPosition);

			return createToken(TOK_IDENTIFIER, currentValue, tokenPosition);
		} 
		// number
		else if (Utils::isDigit(currentChar))
		{
		}
		// special characters
		else
		{
			currentValue.push_back(currentChar);
			if(currentChar == '(')
			{
				return createToken(TOK_ROUND_LEFT_BRACKET, currentValue, tokenPosition);
			}
			else if(currentChar == ')')
			{
				return createToken(TOK_ROUND_RIGHT_BRACKET, currentValue, tokenPosition); 
			}
			else if(currentChar == '#')
			{
				currentChar = currentFile.get();
				++currentPosition.col;

				if(currentChar == '!')
				{
					currentValue.push_back(currentChar);
					return createToken(TOK_COMMENT_BEGIN, currentValue, tokenPosition);
				}
				else
				{
					currentFile.unget();
					--currentPosition.col;
					return createToken(TOK_COMMENT, currentValue, tokenPosition);
				}
			}
			else if(currentChar == '!')
			{
				currentChar = currentFile.get();
				++currentPosition.col;

				if(currentChar == '#') 
				{
					currentValue.push_back(currentChar);
					return createToken(TOK_COMMENT_END, currentValue, tokenPosition);
				}
				else 
				{
					currentFile.unget();
					--currentPosition.col;
					// TODO: make some general errors
					return createToken(TOK_ERROR, currentValue + " Wrong char :(", tokenPosition);
				}
			}
			else if(currentChar == EOF)
			{
				return createToken(TOK_EOF, currentValue, tokenPosition);
			}
		}

		return NULL;
	}

	Token* Scanner::createToken(Type type, std::string value, Position position)
	{
		Token* tempToken = new Token();
		tempToken -> position = position;
		Value tempValue;
		tempValue.type = Value::STRING; // TODO: fix this, should depends on Token Type
		tempValue.value = value;
		tempToken -> value = tempValue;
		tempToken -> type = type;

		return tempToken;
	}
}