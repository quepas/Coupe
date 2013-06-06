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
			
			std::string comparedValue = Utils::toLowerCase(currentValue);

			if(comparedValue == "def")    return createToken(TOK_KW_DEF, currentValue, tokenPosition);
			if(comparedValue == "extern") return createToken(TOK_KW_EXTERN, currentValue, tokenPosition);
			if(comparedValue == "begin")  return createToken(TOK_KW_BEGIN, currentValue, tokenPosition);
			if(comparedValue == "end")    return createToken(TOK_KW_END, currentValue, tokenPosition);
			if(comparedValue == "if")	  return createToken(TOK_KW_IF, currentValue, tokenPosition);
			if(comparedValue == "else")	  return createToken(TOK_KW_ELSE, currentValue, tokenPosition);
			if(comparedValue == "loop")   return createToken(TOK_KW_LOOP, currentValue, tokenPosition);
			if(comparedValue == "return") return createToken(TOK_KW_RETURN, currentValue, tokenPosition);
			if(comparedValue == "and")    return createToken(TOK_KW_AND, currentValue, tokenPosition);
			if(comparedValue == "or")     return createToken(TOK_KW_OR, currentValue, tokenPosition);

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
			else if(currentChar == '*')
			{
				return createToken(TOK_OP_MUL, currentValue, tokenPosition);
			}
			else if(currentChar == '/')
			{
				return createToken(TOK_OP_DIV, currentValue, tokenPosition);
			}
			else if(currentChar == '%')
			{
				return createToken(TOK_OP_MOD, currentValue, tokenPosition);
			}
			else if(currentChar == '-')
			{
				currentChar = currentFile.get();
				++currentPosition.col;

				if(currentChar == '>')
				{
					currentValue.push_back(currentChar);
					return createToken(TOK_OP_IMPLICATION, currentValue, tokenPosition);
				}
				currentFile.unget();
				--currentPosition.col;

				return createToken(TOK_OP_SUB, currentValue, tokenPosition);
			}
			else if(currentChar == '+')
			{
				return createToken(TOK_OP_ADD, currentValue, tokenPosition);
			}
			else if(currentChar == '^')
			{
				return createToken(TOK_OP_POWER, currentValue, tokenPosition);
			}
			else if(currentChar == '#')
			{
				currentChar = currentFile.get();
				currentValue.push_back(currentChar);
				++currentPosition.col;												

				if(currentChar == '!')
				{					
					// TODO: look for '!#'
					do
					{
						currentChar = currentFile.get();
						currentValue.push_back(currentChar);
						++currentPosition.col;

						if(currentChar == '\n')
						{
							++currentPosition.row;
							currentPosition.col = 0;
						}

						if(currentChar == EOF)
						{
							currentFile.unget();
							--currentPosition.col;
							return createToken(TOK_ERROR, currentValue + " - missing !#", tokenPosition);
						}

						if(currentChar == '!')
						{
							currentChar = currentFile.get();
							currentValue.push_back(currentChar);
							++currentPosition.col;

							if(currentChar == '#')
							{
								return createToken(TOK_COMMENT_SECTION, currentValue, tokenPosition);
							}

							if(currentChar == '\n')
							{
								++currentPosition.row;
								currentPosition.col = 0;
							}

							if(currentChar == EOF)
							{
								currentFile.unget();
								--currentPosition.col;
								return createToken(TOK_ERROR, currentValue + " - missing !#", tokenPosition);
							}
						}
					} while(true);					
				}				
				else
				{					
					// TODO: make a method - if EOF then break
					if(currentChar == EOF)
					{
						currentFile.unget();
						--currentPosition.col;					
					}
					else 
					{
						while(currentChar != '\n')
						{
							currentChar = currentFile.get();
							if(currentChar != '\n')
								currentValue.push_back(currentChar);
						}					
						++currentPosition.row;
						currentPosition.col = 0;
					}					

					return createToken(TOK_COMMENT_LINE, currentValue, tokenPosition);
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