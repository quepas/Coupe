#include "CoupeParser.h"
#include "CoupeCodeGen.h"

#include "CoupeUtils.h"
#include <iostream>

namespace Coupe
{
	std::map<Type, int> Parser::binaryOperatorsPrecedence;

	void Parser::initBinaryOpPrec() {		
		binaryOperatorsPrecedence[TOK_OP_LESS] = 10;
		binaryOperatorsPrecedence[TOK_OP_MORE] = 10;
		binaryOperatorsPrecedence[TOK_OP_IMPLICATION] = 20;
		binaryOperatorsPrecedence[TOK_OP_POWER] = 40;
		binaryOperatorsPrecedence[TOK_OP_ADD] = 80;
		binaryOperatorsPrecedence[TOK_OP_SUB] = 80;
		binaryOperatorsPrecedence[TOK_OP_MUL] = 160;
		binaryOperatorsPrecedence[TOK_OP_DIV] = 160;
		binaryOperatorsPrecedence[TOK_OP_MOD] = 160;
	}

	int Parser::getBinaryOpPrec(Type type)
	{
		if(isBinaryOp(type))
		{
			return binaryOperatorsPrecedence[type];
		}
		return -1;
	}

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
		beVerbose(verbose);
	}

	void Parser::beVerbose(bool _verbose)
	{
		verbose = _verbose;
		scanner -> beVerbose(_verbose);
		CodeGen::getInstance().beVerbose(_verbose);
	}

	Token* Parser::getNextToken()
	{
		return token = scanner -> getNext();
	}

	void Parser::parse()
	{
		if(scanner)
		{					
			getNextToken();
			do 
			{												
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
					case TOK_SEMICOLON:
						getNextToken();
						break;
					default:
						handleMainCode();															
						break;
				}
			} while (token -> type != TOK_EOF);			
		}
	}	

	void Parser::handleImport()
	{		
		ImportAST* result = parseImport();
		if(result)
			beVerboseAboutImport(result);	
	}

	ImportAST* Parser::parseImport()
	{		
		getNextToken();
		if(token -> type == TOK_IDENTIFIER)
		{
			std::string importName = token -> value.data;	
			getNextToken(); // eat identifier
			return new ImportAST(importName);
		}
		else
		{			
			return errorI("package name to import excepted", token -> position);
		}				
	}

	void Parser::handleExtern()
	{		
		PrototypeAST* prototype = parseExtern();
		if(prototype)
		{
			llvm::Function* LF = prototype -> Codegen();
			if(LF)
			{
				beVerboseAboutPrototype(prototype, true);
				LF -> dump();
			}			
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
		getNextToken(); // eat ')'
		return new PrototypeAST(functionName, args);
	}
	
	void Parser::handleDefinition()
	{		
		FunctionAST* function = parseDefinition();
		if(function)
		{
			llvm::Function* LF = function -> Codegen();
			if(LF)
			{
				beVerboseAboutFunction(function);
				LF -> dump();
			}
		}
	}

	FunctionAST* Parser::parseDefinition()
	{
		getNextToken();	// eat before
		PrototypeAST* prototype = parsePrototype();
		if(prototype)
		{			
			ExpressionAST* body = parseExpression();
			if(body)
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
		if(!LHS) return nullptr;			
		return parseBinOpRHS(0, LHS);
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
			case TOK_STRING:
				return parseString();
			default:
				return error("Unknown token when expecting expression", token -> position);
		}
	}

	ExpressionAST* Parser::parseIdentifier()
	{		
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
					if(!arg) return nullptr;
					args.push_back(arg);

					if(token -> type == TOK_ROUND_RIGHT_BRACKET)					
						break;					
					if(token -> type != TOK_COMMA)					
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
				result = new NumberAST(lexical_cast<int>(token -> value.data));
				break;
			case TOK_DOUBLE:				
				result = new NumberAST(lexical_cast<double>(token -> value.data));
				break;
		}
		getNextToken(); // eat number
		return result;
	}

	ExpressionAST* Parser::parseString()
	{
		ExpressionAST* result = nullptr;
		result = new StringAST(token->value.data);
		getNextToken(); // eat number
		return result;
	}

	ExpressionAST* Parser::parseParenthesis()
	{
		getNextToken(); // eat '('
		ExpressionAST* result = parseExpression();
		if(!result) return nullptr;

		if(token -> type != TOK_ROUND_RIGHT_BRACKET)
			return error("expected ')'");
		getNextToken(); // eat ')'

		return result;
	}

	ExpressionAST* Parser::parseBinOpRHS(int expressionPrec, ExpressionAST* LHS)
	{
		while(true)
		{
			int tokenPrec = getBinaryOpPrec(token -> type);

			if(tokenPrec < expressionPrec)
				return LHS;

			Type binaryOpType = token -> type;
			getNextToken();						

			ExpressionAST* RHS = parsePrimary();
			if(!RHS) return nullptr;

			int nextTokenPrec = getBinaryOpPrec(token -> type);
			if(tokenPrec < nextTokenPrec)
			{
				RHS = parseBinOpRHS(tokenPrec+1, RHS);
				if(!RHS) return nullptr;
			}

			LHS = new BinaryOpAST(binaryOpType, LHS, RHS);			
		}		
	}

	void Parser::handleMainCode()
	{
		FunctionAST* anonymousFunction = parseTopLevelExpression();

		if(anonymousFunction)
		{
			llvm::Function* LF = anonymousFunction -> Codegen();
			if(LF)
			{
					void *FPtr = TheExecutionEngine->getPointerToFunction(LF);
					llvm::Type* typ = LF->getReturnType();

				 if (typ->isDoubleTy()){
					  double (*FP)() = (double (*)())(intptr_t)FPtr;
					  fprintf(stderr, "Evaluated to %f\n", FP());
				  }
				  else if (typ->isIntegerTy()){
					  int32_t (*FP)() = (int32_t (*)())(intptr_t)FPtr;
					  fprintf(stderr, "Evaluated to %d\n", FP());
				  }
				  else if (typ->isPointerTy()){
					  char* (*FP)() = (char* (*)())(intptr_t)FPtr;
					  fprintf(stderr, "Evaluated to %s\n", FP());
				  }
				beVerboseAboutExpression(anonymousFunction -> getBody());
				LF -> dump();
			}			
		}
	}

	FunctionAST* Parser::parseTopLevelExpression()
	{
		ExpressionAST* instructions = parseExpression();
				
		if(instructions)
		{
			PrototypeAST* anonymousPrototype = 
				new PrototypeAST("", std::vector<std::string>(), instructions -> Codegen() -> getType());
			return new FunctionAST(anonymousPrototype, instructions);
		}
		return nullptr;
	}

	bool Parser::isBinaryOp(Type type)
	{
		switch(type)
		{			
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
			for(unsigned int i = 0; i < args.size(); ++i) {
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