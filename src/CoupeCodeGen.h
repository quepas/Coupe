#ifndef COUPE_CODE_GEN_H_
#define COUPE_CODE_GEN_H_

#include "CoupeValueDef.h"
#include "AST/CoupeASTDef.h"

#include <llvm/Value.h>
#include <llvm/Module.h>
#include <llvm/IRBuilder.h>
#include <llvm/LLVMContext.h>
#include <llvm/Constants.h>

#include <iostream>
#include <vector>
#include <map>

namespace Coupe
{
	class ExpressionAST;
	class PrototypeAST;

	// instnace of CodeGen for one named TheModule
	class CodeGen
	{
		public:		
			void setOutputStream(std::ostream& stream);

			// TODO: what about int?
			llvm::Value* generateNumber(double number);
			llvm::Value* generateVariable(std::string name);
			llvm::Value* generateBinaryOp(char op, ExpressionAST* LHS, ExpressionAST* RHS);
			llvm::Value* generateCall(std::string callee, const std::vector<ExpressionAST*>& args);
			llvm::Value* generatePrototype(std::string name, const std::vector<std::string>& args);
			llvm::Value* generateFunction(PrototypeAST* prototype, ExpressionAST* body);
			llvm::Value* generateImport(std::string name);
			
			llvm::Value* errorV(std::string msg);

			static CodeGen& getInstance()
			{
				static CodeGen instance;
				return instance;
			}
		private:
			CodeGen() : outputStream(&std::cout),
						mainModule(nullptr),
						builder(llvm::getGlobalContext()) {}
			CodeGen(const CodeGen&) : builder(llvm::getGlobalContext()) {}
			CodeGen& operator=(const CodeGen&) {}
			
			std::ostream* outputStream;

			llvm::Module* mainModule;
			llvm::IRBuilder<> builder;
			std::map<std::string, llvm::Value*> namedValues;			

	};
}

#endif