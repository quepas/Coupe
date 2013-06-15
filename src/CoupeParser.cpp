#include "CoupeParser.h"

#include "CoupeUtils.h"
#include <iostream>

namespace Coupe
{
	void Parser::setInputFile(std::string filename)
	{
		std::ifstream* currentFile = new std::ifstream(filename);
		if(!currentFile -> is_open())
		{
			*outputStream << "Cannot open file: " << filename << "!" << std::endl;
		}
		setInputStream(*currentFile);
	}

	void Parser::setInputStream(std::istream& _stream)
	{
		inputStream = &_stream;
		scanner = new Scanner();
		scanner -> setInputStream(*inputStream);
		scanner -> beVerbose(verbose);
	}

	void Parser::beVerbose(bool _verbose)
	{
		verbose = _verbose;
		scanner -> beVerbose(_verbose);
	}

	Token* Parser::getNextToken()
	{
		return token = scanner -> getNext();
	}

	void Parser::parse()
	{
		if(scanner != nullptr)
		{					
			do 
			{								
				// main products (coupe := ...)
				getNextToken();
				switch(token -> type)
				{
					case TOK_KW_IMPORT:
						handleImport();
						break;
					case TOK_KW_EXTERN:
						handleExtern();
						break;
					case TOK_KW_DEF:
						handleDefinition();
						break;
					default:
						//handleMainCode();			
						break;
				}
			} while (token -> type != TOK_EOF);			
		}
	}	

	void Parser::handleImport()
	{		
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
			std::string importName = token -> value.data;	
			//getNextToken(); // eat identifier
			return new ImportAST(importName);
		}
		else
		{			
			return errorI("package name to import excepted", token -> position);
		}				
	}

	void Parser::handleExtern()
	{		
		PrototypeAST* result = parseExtern();
		if(result != nullptr)
		{
			beVerboseAboutPrototype(result, true);
		}
	}

	PrototypeAST* Parser::parseExtern()
	{
		getNextToken();
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
		//getNextToken(); // eat some ')'
		return new PrototypeAST(functionName, args);
	}
	
	void Parser::handleDefinition()
	{		
		FunctionAST* result = parseDefinition();
		if(result != nullptr)
		{
			beVerboseAboutFunction(result);
		}
	}

	FunctionAST* Parser::parseDefinition()
	{
		getNextToken();	// eat before
		PrototypeAST* prototype = parsePrototype();
		if(prototype != nullptr)
		{
			getNextToken();
			ExpressionAST* body = parseExpression();
			if(body != nullptr)
			{
				getNextToken();
				return new FunctionAST(prototype, body);
			}
			else 
			{
				errorF("no function body near place", token -> position);
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

		//getNextToken();	
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
			return new VariableAST(identifier);
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
			if(!isBinaryOperator(token -> type))
				return LHS;

			if(token -> type != TOK_ROUND_RIGHT_BRACKET)
			{
				getNextToken();
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
		// TODO:
		*outputStream << "handleMainCode" << std::endl;
	}

	// error functions
	ExpressionAST* Parser::error(std::string msg, Position position /* = Position(0, 0) */)
	{
		showErrorMessage(msg, position);
		return nullptr;
	}

	ImportAST* Parser::errorI(std::string msg, Position position /* = Position(0, 0) */)
	{
		showErrorMessage(msg, position);
		return nullptr;
	}

	PrototypeAST* Parser::errorP(std::string msg, Position position /* = Position(0, 0) */)
	{
		showErrorMessage(msg, position);
		return nullptr;
	}

	FunctionAST* Parser::errorF(std::string msg, Position position /* = Position(0, 0) */)
	{
		showErrorMessage(msg, position);
		return nullptr;
	}

	void Parser::showErrorMessage(std::string msg, Position position)
	{
		msg = "Error: " + msg;
		if(position.row != 0 || position.col != 0)
		{
			msg.append(msg + " at [" + lexical_cast<std::string>(position.row) 
				+ ", " + lexical_cast<std::string>(position.col) + "]");
		}
		*outputStream << msg << std::endl;
	}
	
	void Parser::beVerboseAboutImport(ImportAST* import)
	{
		if(verbose)
		{
			*outputStream << "[Import]: Successfully parsed!" << std::endl;
			*outputStream << "\tName: " << import -> getName() << std::endl;
		}		
	}

	void Parser::beVerboseAboutPrototype(PrototypeAST* prototype, bool isExternal /* = false */)
	{
		if(verbose)
		{			
			*outputStream << "[Prototype]: Successfully parsed!" << std::endl;
			*outputStream << "\tName: " << prototype -> getName() << std::endl;
			*outputStream << "\tExternal: " << (isExternal ? "yes" : "no") << std::endl;
			*outputStream << "\tArguments: ";
			
			std::vector<std::string> args = prototype -> getArgs();
			for(int i = 0; i < args.size(); ++i) {
				*outputStream << args[i] << (i+1 != args.size() ? ", " : "\n");				 				
			}
		}		
	}

	void Parser::beVerboseAboutFunction(FunctionAST* function)
	{
		if(verbose)
		{
			*outputStream << "--------------------------------------------------" << std::endl;
			*outputStream << "[Function]: Successfully parsed!" << std::endl; 
			beVerboseAboutPrototype(function -> getPrototype());
			beVerboseAboutExpression(function -> getBody());
			*outputStream << "--------------------------------------------------" << std::endl;
		}
	}

	void Parser::beVerboseAboutExpression(ExpressionAST* expression)
	{
		if(verbose)
		{
			// TODO:
			*outputStream << "being verbose about expression" << std::endl;			
		}
	}
}