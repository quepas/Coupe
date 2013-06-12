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
			getNextToken();
			do 
			{				
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

	Token* Parser::getNextToken()
	{
		return token = scanner -> getNext();
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
			getNextToken();
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
		getNextToken();	// eat before
		return parsePrototype();
	}

	PrototypeAST* Parser::parsePrototype()
	{
		std::string functionName;
		std::vector<std::string> args;
		
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
		getNextToken(); // eat some ')'
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
		getNextToken();	// eat before
		PrototypeAST* prototype = parsePrototype();
		if(prototype != nullptr)
		{
			ExpressionAST* body = parseExpression();
			if(body != nullptr)
			{
				getNextToken();
				return new FunctionAST(prototype, body);
			}
			else 
			{
				errorF("Warning: no function body near place", token -> position);
			}
		}	
		return nullptr;
	}

	ExpressionAST* Parser::parseExpression()
	{
		ExpressionAST* LHS = parsePrimary();
		if(LHS == nullptr) 
		{
			return nullptr;
		}

		getNextToken();	
		// TODO: refractor, end of expression should be find better
		if(isBinaryOperator(token -> type))
		{
			return parseBinOpRHS(0, LHS);
		}
		return LHS;
	}

	ExpressionAST* Parser::parsePrimary()
	{
		switch(token -> type)
		{
			case TOK_IDENTIFIER:
				return parseIdentifier();
			case TOK_INTEGER:
				return parseNumber();
			case TOK_DOUBLE:
				return parseNumber();
			case TOK_ROUND_LEFT_BRACKET:
				return parseParenthesis();
			default:
				return error("Unknown token when expecting expression", token -> position);
		}
	}

	ExpressionAST* Parser::parseIdentifier()
	{
		// TODO: check if String
		std::string identifier = token -> value.data;
		getNextToken();	// eat identifier

		if(token -> type != TOK_ROUND_LEFT_BRACKET)
		{
			new VariableAST(identifier);
		} 
		else
		{
			getNextToken(); // eat '('
			std::vector<ExpressionAST*> args;

			if(token -> type != TOK_ROUND_RIGHT_BRACKET)
			{
				while(true)
				{
					ExpressionAST* arg = parseExpression();
					if(arg == nullptr)
						return nullptr;
					args.push_back(arg);

					if(token -> type == TOK_ROUND_RIGHT_BRACKET)					
						break;					
					if(token -> type != ',')					
						return error("expected ')' or ',' in argument list", token -> position);					
					getNextToken();
				}
			}
			getNextToken(); // Eat ')'
			return new CallAST(identifier, args);
		}		
	}

	ExpressionAST* Parser::parseNumber()
	{
		ExpressionAST* result = nullptr;
		switch(token -> type)
		{
			case TOK_INTEGER:
				return new NumberAST(lexical_cast<int>(token -> value.data));
			case TOK_DOUBLE:
				return new NumberAST(lexical_cast<double>(token -> value.data));
		}
		getNextToken(); // eat number
		return result;
	}

	ExpressionAST* Parser::parseParenthesis()
	{
		getNextToken(); // eat '('
		ExpressionAST* expression = parseExpression();
		if(!expression)
			return nullptr;

		if(token -> type != TOK_ROUND_RIGHT_BRACKET)
			return error("expected ')'");
		getNextToken(); // eat ')'

		return expression;
	}

	ExpressionAST* Parser::parseBinOpRHS(int expressionPrec, ExpressionAST* LHS)
	{
		while(true)
		{
			int tokenPrec = 10; // TODO: fixme

			if(tokenPrec < expressionPrec)
				return LHS;

			//Token* binaryOp = token;
			char binaryOp = '+';
			getNextToken();

			if(token -> type != TOK_ROUND_RIGHT_BRACKET)
			{
				ExpressionAST* RHS = parsePrimary();
				if(!RHS)
					return nullptr;

				int nextTokenPrec = 10; // TODO: fixme
				if(tokenPrec < nextTokenPrec)
				{
					RHS = parseBinOpRHS(tokenPrec+1, RHS);
					if(RHS == nullptr)
						return nullptr;
				}
				LHS = new BinaryOpAST(binaryOp, LHS, RHS);
			}
			else
			{
				break;
			}	
		}
		return nullptr;
	}

	bool Parser::isBinaryOperator(Type type)
	{
		switch(type)
		{
			case TOK_KW_AND:
			case TOK_KW_OR:
			case TOK_OP_IMPLICATION:
			case TOK_OP_MUL:
			case TOK_OP_DIV:
			case TOK_OP_MOD:
			case TOK_OP_SUB:
			case TOK_OP_ADD:
			case TOK_OP_POWER:
			case TOK_OP_LESS:
			case TOK_OP_MORE:
				return true;
			default:
				return false;
		}		
	}

	void Parser::handleMainCode()
	{
		beVerboseAboutHandling("MainCode");		
	}

	// error functions
	ExpressionAST* Parser::error(std::string msg, Position position /* = Position(0, 0) */)
	{
		*outputStream << prepareErrorMsg(msg, position) << std::endl;
		return nullptr;
	}

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

	FunctionAST* Parser::errorF(std::string msg, Position position /* = Position(0, 0) */)
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