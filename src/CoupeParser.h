#ifndef COUPE_PARSER_H_
#define COUPE_PARSER_H_

#include "AST/CoupeASTDef.h"
#include "CoupeScanner.h"

namespace Coupe
{
	class Parser
	{
		public:
			Parser() : currentStream(nullptr) {}

			void setFile(std::string filename);
			void setStream(std::istream stream);

			void parse();

			// handling main products
			void handleImport();
			void handleExternal();
			void handleDefinition();			
			void handleMainCode();			
			
		private:
			Scanner scanner;	
			std::istream* currentStream;

			// each method represent parsing one grammar product
			ExpressionAST* parseNumber();
			ExpressionAST* parseIdentifier();
			ExpressionAST* parseParenthesis();
			ExpressionAST* parsePrimary();
			ExpressionAST* parseBinOpRHS(int expressionPrec, ExpressionAST* LHS);
			PrototypeAST* parsePrototype();
			FunctionAST* parseFunction();
			FunctionAST* parseTopLevelExpression();
	};
}

#endif