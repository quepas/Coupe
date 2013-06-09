#ifndef COUPE_PARSER_H_
#define COUPE_PARSER_H_

#include "AST/CoupeASTDef.h"
#include "CoupeScanner.h"

namespace Coupe
{
	class Parser
	{
		public:
			Parser() : inputStream(&std::cin),
					   outputStream(&std::cout),
					   token(nullptr),
					   scanner(nullptr),
					   verbose(false) {}

			void setInputFile(std::string filename);
			void setInputStream(std::istream& stream);
			void setOutputStream(std::ostream& stream);
			void setVerbose(bool verbose);
			void parse();
			
		private:
			Scanner* scanner;	
			std::istream* inputStream;
			std::ostream* outputStream;
			Token* token;
			bool verbose;

			void getNextToken();

			// handling main products
			void handleImport();
			void handleExternal();
			void handleDefinition();			
			void handleMainCode();			

			// each method represent parsing one grammar product		
			ExpressionAST* parseNumber();
			ExpressionAST* parseIdentifier();
			ExpressionAST* parseParenthesis();
			ExpressionAST* parsePrimary();
			ExpressionAST* parseExpression();
			ExpressionAST* parseBinOpRHS(int expressionPrec, ExpressionAST* LHS);
			PrototypeAST* parsePrototype();
			PrototypeAST* parseExtern();
			FunctionAST* parseDefinition();
			FunctionAST* parseTopLevelExpression();
			ImportAST* parseImport();

			// error functions
			ExpressionAST* error(std::string msg, Position position = Position(0, 0));
			PrototypeAST* errorP(std::string msg, Position position = Position(0, 0));
			FunctionAST* errorF(std::string msg, Position position = Position(0, 0));
			ImportAST* errorI(std::string msg, Position position = Position(0, 0));
			std::string prepareErrorMsg(std::string msg, Position position);

			// verbose functions			
			void beVerboseAboutHandling(std::string name);
			void beVerboseAboutExpression(ExpressionAST* expression);
			void beVerboseAboutPrototype(PrototypeAST* prototype);
			void beVerboseAboutDefinition(FunctionAST* function);
			void beVerboseAboutImport(ImportAST* import);
	};
}

#endif