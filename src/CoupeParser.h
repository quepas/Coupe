#ifndef COUPE_PARSER_H_
#define COUPE_PARSER_H_

#include <map>

#include "CoupeScanner.h"
#include "CoupeCodeGen.h"

#include "AST/CoupeASTDef.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"

namespace Coupe
{
	class Parser
	{
		public:
			Parser() : scanner(new Scanner()),
					   codeGen(CodeGen::getInstance()),
					   token(nullptr),
					   verbose(false),
					   verboseEval(false),
					   useShell(true),
					   inputStream(&std::cin),
					   outputStream(&std::cout)
			{ initBinaryOpPrec(); }

			void setInputFile(std::string filename);
			void setInputStream(std::istream& stream);
			void setOutputStream(std::ostream& stream);
			void beVerbose(bool verbose);
			void beVerboseAboutEvaluation(bool verboseEval);
			void beVerboseAboutScanner(bool verbose);
			void beVerboseAboutCodeGen(bool verbose);	
			void useAsShell(bool _useAsShell);
			void parse();	
			bool parseAndExecuteShellCommand();
			
		private:
			Scanner* scanner;	
			CodeGen& codeGen;

			Token* token;
			bool verbose;
			bool verboseEval;
			bool useShell;
			std::istream* inputStream;
			std::ostream* outputStream;		
			static std::map<Type, int> binaryOperatorsPrecedence;

			Token* getNextToken();

			// handling main products
			void handleImport();
			void handleExtern();
			void handleDefinition();			
			void handleMainCode();			

			// parsing sub-product		
			ExpressionAST* parseNumber();
			ExpressionAST* parseString();
			ExpressionAST* parseIdentifier();
			ExpressionAST* parseParenthesis();
			ExpressionAST* parsePrimary();
			ExpressionAST* parseExpression();
			ExpressionAST* parseBinOpRHS(int expressionPrec, ExpressionAST* LHS);
			ExpressionAST* parsePattern();
			PrototypeAST* parsePrototype();
			PrototypeAST* parseExtern();
			FunctionAST* parseDefinition();
			FunctionAST* parseTopLevelExpression();
			ImportAST* parseImport();

			// helper functions
			bool isBinaryOp(Type type);
			static void initBinaryOpPrec();
			int getBinaryOpPrec(Type type);

			// error functions
			ExpressionAST* error(std::string msg, Position position = Position(0, 0));
			PrototypeAST* errorP(std::string msg, Position position = Position(0, 0));
			FunctionAST* errorF(std::string msg, Position position = Position(0, 0));
			ImportAST* errorI(std::string msg, Position position = Position(0, 0));
			void showErrorMessage(std::string msg, Position position);

			// verbose functions						
			void beVerboseAboutExpression(ExpressionAST* expression);
			void beVerboseAboutPrototype(PrototypeAST* prototype, bool isExternal = false);
			void beVerboseAboutFunction(FunctionAST* function);
			void beVerboseAboutImport(ImportAST* import);
			void beVerboseAboutEvaluation(llvm::Function* function);		
			void beVerboseAboutDump(llvm::Function* function);

			void executeCoupeFromFile(std::string filename);
	};
}

#endif