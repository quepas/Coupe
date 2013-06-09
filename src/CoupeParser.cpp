#include "CoupeParser.h"

#include "CoupeUtils.h"
#include <iostream>

namespace Coupe
{
	void Parser::setInputFile(std::string filename)
	{
		std::ifstream* currentFile = new std::ifstream();
		currentFile -> open(filename);
		if(!currentFile -> is_open())
		{
			std::cout << "Cannot open file: " << filename << "!" << std::endl;
		}
		setInputStream(*currentFile);
	}

	void Parser::setInputStream(std::istream& _stream)
	{
		inputStream = &_stream;
		scanner = new Scanner();
		scanner -> setInputStream(*inputStream);
	}

	void Parser::setVerbose(bool _verbose)
	{
		verbose = _verbose;
	}

	void Parser::parse()
	{
		if(scanner != nullptr)
		{
			do 
			{
				token = scanner -> getNext();

				// main products (coupe := ...)
				switch(token -> type)
				{
					case TOK_KW_IMPORT:
						handleImport();
						break;
					case TOK_KW_EXTERN:
						handleExternal();
						break;
					case TOK_KW_DEF:
						handleDefinition();
						break;
					default:
						handleMainCode();
						break;
				}
			} while (token -> type != TOK_EOF);
		}
	}

	void Parser::getNextToken()
	{
		token = scanner -> getNext();
	}

	void Parser::handleImport()
	{
		beVerboseAboutHandling("Import");
		ImportAST* result = parseImport();
		if(result != nullptr)
		{
			beVerboseAboutImport(result);
		}		
	}

	ImportAST* Parser::parseImport()
	{		
		getNextToken();

		if(token -> type == TOK_IDENTIFIER)
		{
			// TODO: mind about correct value.type 
			return new ImportAST(token -> value.data);
		}
		else
		{			
			return errorI("package name to import excepted", token -> position);
		}				
	}

	void Parser::handleExternal()
	{
		beVerboseAboutHandling("External");
		PrototypeAST* result = parseExtern();
		if(result != nullptr)
		{
			beVerboseAboutPrototype(result);
		}
	}

	PrototypeAST* Parser::parseExtern()
	{
		return parsePrototype();
	}

	PrototypeAST* Parser::parsePrototype()
	{
		std::string functionName;
		std::vector<std::string> args;

		getNextToken();
		if(token -> type != TOK_IDENTIFIER)
		{
			return errorP("expected function name in prototype", token -> position);
		}
		functionName = token -> value.data;

		getNextToken();
		if(token -> type != TOK_ROUND_LEFT_BRACKET)
		{
			return errorP("expected '(' in prototype", token -> position);
		}
		
		getNextToken();
		while (token -> type == TOK_IDENTIFIER)
		{
			args.push_back(token -> value.data);
			getNextToken();
		}

		if(token -> type != TOK_ROUND_RIGHT_BRACKET)
		{
			return errorP("expected ')' in prototype", token -> position);
		}
		return new PrototypeAST(functionName, args);
	}
	
	void Parser::handleDefinition()
	{
		beVerboseAboutHandling("Definition");
		FunctionAST* result = parseDefinition();
		if(result != nullptr)
		{
			beVerboseAboutDefinition(result);
		}
	}

	FunctionAST* Parser::parseDefinition()
	{
		PrototypeAST* prototype = parsePrototype();
		if(prototype != nullptr)
		{
			ExpressionAST* body = parseExpression();
			if(body != nullptr)
			{
				return new FunctionAST(prototype, body);
			}
		}	
		return nullptr;
	}

	ExpressionAST* Parser::parseExpression()
	{
		return nullptr;
	}

	void Parser::handleMainCode()
	{
		beVerboseAboutHandling("MainCode");
	}

	// error functions
	ImportAST* Parser::errorI(std::string msg, Position position /* = Position(0, 0) */)
	{
		*outputStream << prepareErrorMsg(msg, position) << std::endl;
		return nullptr;
	}

	PrototypeAST* Parser::errorP(std::string msg, Position position /* = Position(0, 0) */)
	{
		*outputStream << prepareErrorMsg(msg, position) << std::endl;
		return nullptr;
	}

	std::string Parser::prepareErrorMsg(std::string msg, Position position)
	{
		msg = "Error: " + msg;
		if(position.row != 0 || position.col != 0)
		{
			return msg + " at [" + lexical_cast<std::string>(position.row) 
				+ ", " + lexical_cast<std::string>(position.col) + "]";
		}
		return msg;
	}

	void Parser::beVerboseAboutHandling(std::string name)
	{
		if(verbose)
		{
			*outputStream << "Parser::handle" << name << "()" << std::endl;
		}		
	}

	void Parser::beVerboseAboutImport(ImportAST* import)
	{
		if(verbose)
		{
			*outputStream << "beVerboseAboutImport() // TODO" << std::endl;
		}		
	}

	void Parser::beVerboseAboutPrototype(PrototypeAST* prototype)
	{
		if(verbose)
		{
			*outputStream << "beVerboseAboutPrototype() // TODO" << std::endl;
		}		
	}

	void Parser::beVerboseAboutDefinition(FunctionAST* definition)
	{
		if(verbose)
		{
			*outputStream << "beVerboseAboutDefinition() // TODO" << std::endl; 
		}
	}
}