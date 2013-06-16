#ifndef COUPE_CODE_GEN_H_
#define COUPE_CODE_GEN_H_

#include "AST/CoupeASTDef.h"

#include <llvm/Value.h>
#include <llvm/Module.h>
#include <llvm/IRBuilder.h>
#include <llvm/LLVMContext.h>
#include <llvm/Constants.h>
#include "llvm/PassManager.h"

#include <iostream>
#include <vector>
#include <map>

namespace Coupe
{
	struct NumberValue;
	class ExpressionAST;
	class PrototypeAST;

	// instnace of CodeGen for one named TheModule
	class CodeGen
	{
		public:		
			void setOutputStream(std::ostream& stream);

			// TODO: what about int?
			llvm::Value* generateNumber(NumberValue number);
			llvm::Value* generateString(std::string value);
			llvm::Value* generateVariable(std::string name);
			llvm::Value* generateBinaryOp(Type op, ExpressionAST* LHS, ExpressionAST* RHS);
			llvm::Value* generateCall(std::string callee, const std::vector<ExpressionAST*>& args);
			llvm::Function* generatePrototype(std::string name, const std::vector<std::string>& args, llvm::Type* returnType = nullptr);
			llvm::Function* generateFunction(PrototypeAST* prototype, ExpressionAST* body);
			llvm::Value* generateImport(std::string name);
			
			llvm::Value* errorV(std::string msg);
			llvm::Function* errorF(std::string msg);

			void beVerbose(bool verbose);

			llvm::FunctionPassManager *TheFPM;
			llvm::Module* mainModule;

			static CodeGen& getInstance()
			{
				static CodeGen instance;
				return instance;
			}
		private:
			CodeGen() : outputStream(&std::cout),
						mainModule(new llvm::Module("CoupeModule", llvm::getGlobalContext())),
						builder(llvm::getGlobalContext()),
						verbose(false) {}
			CodeGen(const CodeGen&) : builder(llvm::getGlobalContext()) {}
			CodeGen& operator=(const CodeGen&) {}
			
			std::ostream* outputStream;
			bool verbose;

		
			llvm::IRBuilder<> builder;
			std::map<std::string, llvm::Value*> namedValues;

			void beVerboseAbout(std::string msg);
	};
}

#endif