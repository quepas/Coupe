#include "CoupeScanner.h"

#include "CoupeUtils.h"

namespace Coupe
{
	void Scanner::setInputFile(std::string fileName)
	{		
		std::ifstream* file = new std::ifstream(fileName);
		if(!file -> is_open()) 
		{
			*outputStream << "Cannot open file: " << fileName << "!" << std::endl;
		}
		setInputStream(*file);
		position.set(1, 0);
	}

	void Scanner::setInputStream(std::istream& stream)
	{
		inputStream = &stream;
	}

	void Scanner::beVerbose(bool _verbose)
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
			currentChar = getCharacter();

			if(currentChar == '\n')
			{
				toNextRow();
			}
		} while (Utils::isWhitespace(currentChar));
		tokenPosition.set(position.row, position.col);

		// identifier or keyword
		if(Utils::isCharacter(currentChar)) 
		{
			do 
			{
				currentValue.push_back(currentChar);
				currentChar = getCharacter();
			} while (Utils::isCharacter(currentChar) || Utils::isDigit(currentChar) || currentChar == '_');
			ungetCharacter();
			
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
		else if(Utils::isDigit(currentChar))
		{			
			bool isDouble = false;

			do 
			{
				currentValue.push_back(currentChar);
				currentChar = getCharacter();

				if(currentChar == '.')
				{
					isDouble = true;
					currentValue.push_back(currentChar);
					currentChar = getCharacter();
				}
			} while (Utils::isDigit(currentChar));
			ungetCharacter();

			if(isDouble)
			{
				return createToken(TOK_DOUBLE, currentValue, tokenPosition);
			}
			return createToken(TOK_INTEGER, currentValue, tokenPosition);
		}
		// special characters
		else
		{
			currentValue.push_back(currentChar);

			switch(currentChar) 
			{
				case '(':
					return createToken(TOK_ROUND_LEFT_BRACKET, currentValue, tokenPosition);
				case ')':
					return createToken(TOK_ROUND_RIGHT_BRACKET, currentValue, tokenPosition); 
				case '*':
					return createToken(TOK_OP_MUL, currentValue, tokenPosition);
				case '/':
					return createToken(TOK_OP_DIV, currentValue, tokenPosition);
				case '%':
					return createToken(TOK_OP_MOD, currentValue, tokenPosition);
				case '-':
					currentChar = getCharacter();

					if(currentChar == '>')
					{
						currentValue.push_back(currentChar);
						return createToken(TOK_OP_IMPLICATION, currentValue, tokenPosition);
					}
					ungetCharacter();

					return createToken(TOK_OP_SUB, currentValue, tokenPosition);
				case '+':
					return createToken(TOK_OP_ADD, currentValue, tokenPosition);
				case '^':
					return createToken(TOK_OP_POWER, currentValue, tokenPosition);
				case ',':
					return createToken(TOK_COMMA, currentValue, tokenPosition);
				case '<':
					return createToken(TOK_OP_LESS, currentValue, tokenPosition);
				case '>':
					return createToken(TOK_OP_MORE, currentValue, tokenPosition);
				case '#':
					currentChar = getCharacter();
					currentValue.push_back(currentChar);															

					if(currentChar == '!')
					{										
						do
						{
							currentChar = getCharacter();
							currentValue.push_back(currentChar);						

							switch(currentChar)
							{
								case '\n':
									toNextRow();
									break;
								case EOF:
									ungetCharacter();
									return createToken(TOK_ERROR, currentValue + " - missing !#", tokenPosition);
								case '!':
									currentChar = getCharacter();
									currentValue.push_back(currentChar);							

									switch(currentChar)
									{
										case '#':
											return createToken(TOK_COMMENT_SECTION, currentValue, tokenPosition);
										case '\n':
											toNextRow();
											break;
										case EOF:
											ungetCharacter();
											return createToken(TOK_ERROR, currentValue + " - missing !#", tokenPosition);
									}									
							}													
						} while(true);					
					}				
					else
					{										
						if(currentChar == EOF)
						{
							ungetCharacter();			
						}
						else 
						{
							while(currentChar != '\n')
							{
								currentChar = inputStream -> get();
								if(currentChar != '\n')
									currentValue.push_back(currentChar);
							}					
							toNextRow();
						}					

						return createToken(TOK_COMMENT_LINE, currentValue, tokenPosition);
					}
				case EOF:
					return createToken(TOK_EOF, currentValue, tokenPosition);
			}
			return createToken(TOK_ERROR, currentValue, tokenPosition);			
		}
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
			*outputStream << "* " << Utils::createTokenInfo(token) << std::endl;					
	}

	char Scanner::getCharacter()
	{
		++position.col;
		return inputStream -> get();		
	}

	void Scanner::ungetCharacter()
	{
		inputStream -> unget();
		--position.col;
	}

	void Scanner::toNextRow()
	{
		++position.row;
		position.col = 0;
	}
}