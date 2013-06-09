#include "CoupeScanner.h"

#include "CoupeUtils.h"
#include <iostream>
#include <string>

namespace Coupe
{
	// TODO: rewrite - use base istream!
	void Scanner::setInputFile(std::string _filename)
	{		
		std::ifstream* currentFile = new std::ifstream();
		currentFile -> open(_filename);
		if(!currentFile -> is_open()) 
		{
			std::cout << "Cannot open file: " << _filename << "!" << std::endl;
		}
		setInputStream(*currentFile);
		currentPosition.set(1, 0);
	}

	void Scanner::setInputStream(std::istream& stream)
	{
		inputStream = &stream;
	}

	void Scanner::setVerbose(bool _verbose)
	{
		verbose = _verbose;
	}

	Token* Scanner::getNext()
	{
		Token* token = getNextToken();
		beVerboseAboutToken(token);
		return token;
	}

	Token* Scanner::getNextToken()
	{					
		char currentChar;
		std::string currentValue = "";
		Position tokenPosition;
		// get next chars and remove all whitespaces
		do 
		{
			currentChar = inputStream -> get();
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
				currentChar = inputStream -> get();
				++currentPosition.col;
			} while (Utils::isCharacter(currentChar) || Utils::isDigit(currentChar) || currentChar == '_');
			inputStream -> unget();
			--currentPosition.col;
			
			std::string comparedValue = Utils::toLowerCase(currentValue);

			if(comparedValue == "def")		return createToken(TOK_KW_DEF, currentValue, tokenPosition);
			if(comparedValue == "extern")	return createToken(TOK_KW_EXTERN, currentValue, tokenPosition);
			if(comparedValue == "begin")	return createToken(TOK_KW_BEGIN, currentValue, tokenPosition);
			if(comparedValue == "end")		return createToken(TOK_KW_END, currentValue, tokenPosition);
			if(comparedValue == "if")		return createToken(TOK_KW_IF, currentValue, tokenPosition);
			if(comparedValue == "else")		return createToken(TOK_KW_ELSE, currentValue, tokenPosition);
			if(comparedValue == "loop")		return createToken(TOK_KW_LOOP, currentValue, tokenPosition);
			if(comparedValue == "return")	return createToken(TOK_KW_RETURN, currentValue, tokenPosition);
			if(comparedValue == "and")		return createToken(TOK_KW_AND, currentValue, tokenPosition);
			if(comparedValue == "or")		return createToken(TOK_KW_OR, currentValue, tokenPosition);
			if(comparedValue == "import")	return createToken(TOK_KW_IMPORT, currentValue, tokenPosition);

			return createToken(TOK_IDENTIFIER, currentValue, tokenPosition);
		} 
		// number
		else if (Utils::isDigit(currentChar))
		{
			// TODO: add non-fixed numbers processing
			do 
			{
				currentValue.push_back(currentChar);
				currentChar = inputStream -> get();
				++currentPosition.col;
			} while (Utils::isDigit(currentChar));
			inputStream -> unget();
			--currentPosition.col;

			return createToken(TOK_INTEGER, currentValue, tokenPosition);
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
				currentChar = inputStream -> get();
				++currentPosition.col;

				if(currentChar == '>')
				{
					currentValue.push_back(currentChar);
					return createToken(TOK_OP_IMPLICATION, currentValue, tokenPosition);
				}
				inputStream -> unget();
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
				currentChar = inputStream -> get();
				currentValue.push_back(currentChar);
				++currentPosition.col;												

				if(currentChar == '!')
				{					
					// TODO: look for '!#'
					do
					{
						currentChar = inputStream -> get();
						currentValue.push_back(currentChar);
						++currentPosition.col;

						if(currentChar == '\n')
						{
							++currentPosition.row;
							currentPosition.col = 0;
						}

						if(currentChar == EOF)
						{
							inputStream -> unget();
							--currentPosition.col;
							return createToken(TOK_ERROR, currentValue + " - missing !#", tokenPosition);
						}

						if(currentChar == '!')
						{
							currentChar = inputStream -> get();
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
								inputStream -> unget();
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
						inputStream -> unget();
						--currentPosition.col;					
					}
					else 
					{
						while(currentChar != '\n')
						{
							currentChar = inputStream -> get();
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
		tempValue.data = value;
		tempToken -> value = tempValue;
		tempToken -> type = type;

		return tempToken;
	}

	void Scanner::beVerboseAboutToken(Token* token)
	{
		if(verbose)
		{
			*outputStream << Utils::createTokenInfo(token) << std::endl;				
		}		
	}
}