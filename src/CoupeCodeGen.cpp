#include "CoupeCodeGen.h"

#include <llvm/LLVMContext.h>
#include <llvm/Constants.h>

namespace Coupe
{
	void CodeGen::setOutputStream(std::ostream& stream) 
	{
		outputStream = &stream;
	}

	llvm::Value* CodeGen::generateNumber(double number)
	{
		return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(number));
	}

	llvm::Value* CodeGen::generateVariable(std::string name)
	{		
		llvm::Value* result = namedValues[name];		
		return result ? result : errorV("Unknown variable name");
	}

	llvm::Value* CodeGen::generateBinaryOp(char op, ExpressionAST* LHS, ExpressionAST* RHS)
	{
		llvm::Value* L = LHS -> Codegen();
		llvm::Value* R = RHS -> Codegen();

		if(L == nullptr || R == nullptr)
		{
			return nullptr;
		}
		
		llvm::Type* typeL = L -> getType();
		llvm::Type* typeR = R -> getType();

		switch(op)
		{
			case '+':
				/*if(typeL -> isIntegerTy() && typeR -> isIntegerTy())
				{
					return builder.CreateAdd(L, R, "i_addtmp");
				}
				else(typeL -> isDoubleTy() && typeR -> isDoubleTy()) 
				{
					return builder.CreateFAdd(L, R, "d_addtmp");
				}*/
			case '-':
				return nullptr;
			case '*':
				return nullptr;
			case '<':
				return nullptr;
			default:
				return errorV("invalid binary operator");
		}

	}

	llvm::Value* CodeGen::generateCall(std::string callee, const std::vector<ExpressionAST*>& args)
	{
		llvm::Function *calleeF = mainModule -> getFunction(callee);

		if(calleeF == nullptr)
		{
			return errorV("Unknown function referenced");	
		}

		return nullptr;
	}

	llvm::Value* CodeGen::generatePrototype(std::string name, const std::vector<std::string>& args)
	{
		return nullptr;
	}

	llvm::Value* CodeGen::generateFunction(PrototypeAST* prototype, ExpressionAST* body)
	{
		return nullptr;
	}

	llvm::Value* CodeGen::generateImport(std::string name)
	{
		return nullptr;
	}

	llvm::Value* CodeGen::errorV(std::string msg)
	{
		*outputStream << "CodeGen error: " << msg << std::endl;
		return nullptr;
	}
}