#include "CoupeCodeGen.h"

#include <vector>

#include <llvm/LLVMContext.h>
#include <llvm/Constants.h>
#include <llvm/Function.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Analysis/Verifier.h>

namespace Coupe
{
	void CodeGen::setOutputStream(std::ostream& stream) 
	{
		outputStream = &stream;
	}

	llvm::Value* CodeGen::generateNumber(NumberValue number)
	{
		if(number.type == NumberValue::INTEGER)
			return llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, number.value.i));
		else // NumberValue::DOUBLE
			return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(number.value.d));				
	}

	llvm::Value* CodeGen::generateVariable(std::string name)
	{		
		llvm::Value* result = namedValues[name];		
		return result ? result : errorV("Unknown variable name");
	}

	llvm::Value* CodeGen::generateBinaryOp(Type op, ExpressionAST* LHS, ExpressionAST* RHS)
	{
		llvm::Value* L = LHS -> Codegen();
		llvm::Value* R = RHS -> Codegen();

		if(!L || !R) return nullptr;		
		
		llvm::Type* typeL = L -> getType();
		llvm::Type* typeR = R -> getType();

		// TODO: what about expression (1 + 2.0)?
		// TODO2: fill this up!
		if(typeL -> isIntegerTy() && typeR -> isIntegerTy())
		{
			switch(op)
			{
				case TOK_OP_IMPLICATION:
					return errorV("binary operator is not supported yet");
				case TOK_OP_MUL:
					return builder.CreateMul(L, R, "i_multmp");
				case TOK_OP_DIV:
					return builder.CreateSDiv(L, R, "i_sdivtmp");
				case TOK_OP_MOD:
					return errorV("binary operator is not supported yet");
				case TOK_OP_ADD:
					return builder.CreateAdd(L, R, "i_addtmp");
				case TOK_OP_SUB:
					return builder.CreateSub(L, R, "i_subtmp");
				case TOK_OP_POWER:
					return errorV("binary operator is not supported yet");
				case TOK_OP_LESS:
					return errorV("binary operator is not supported yet");
				case TOK_OP_MORE:
					return errorV("binary operator is not supported yet");
				default:
					return errorV("invalid binary operator");
			}
		} 
		else if(typeL -> isDoubleTy() && typeR -> isDoubleTy())
		{
			switch(op)
			{
				case TOK_OP_IMPLICATION:
					return errorV("binary operator is not supported yet");
				case TOK_OP_MUL:
					return builder.CreateFMul(L, R, "d_multmp");
				case TOK_OP_DIV:
					return builder.CreateFDiv(L, R, "d_divtmp");
				case TOK_OP_MOD:
					return errorV("binary operator is not supported yet");
				case TOK_OP_ADD:
					return builder.CreateFAdd(L, R, "d_addtmp");
				case TOK_OP_SUB:
					return builder.CreateFSub(L, R, "d_subtmp");
				case TOK_OP_POWER:
					return errorV("binary operator is not supported yet");
				case TOK_OP_LESS:
					return errorV("binary operator is not supported yet");
				case TOK_OP_MORE:
					return errorV("binary operator is not supported yet");
				default:
					return errorV("invalid binary operator");
			}
		}
	}

	llvm::Value* CodeGen::generateCall(std::string callee, const std::vector<ExpressionAST*>& args)
	{
		llvm::Function *calleeF = mainModule -> getFunction(callee);

		if(!calleeF)
			return errorV("Unknown function referenced");			
		if(calleeF -> arg_size() != args.size())
			return errorV("Incorrect number of arguments passed");

		std::vector<llvm::Value*> argsValues;
		for(unsigned int i = 0; i < args.size(); ++i) 
		{
			argsValues.push_back(args[i] -> Codegen());
			if(!argsValues.back()) return nullptr;
		}
		return builder.CreateCall(calleeF, argsValues, "calltmp");
	}

	llvm::Function* CodeGen::generatePrototype(std::string name, const std::vector<std::string>& args)
	{
		std::vector<llvm::Type*> doubles(args.size(), llvm::Type::getDoubleTy(llvm::getGlobalContext()));
		llvm::FunctionType* functionType = llvm::FunctionType::get(llvm::Type::getDoubleTy(llvm::getGlobalContext()), doubles, false);
		llvm::Function* function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, name,	mainModule);

		if(function -> getName() != name)
		{
			function -> eraseFromParent();
			function = mainModule -> getFunction(name);

			if(!function -> empty()) 
				errorF("redefinition of function");
			if(function -> arg_size() != args.size()) 
				errorF("redefinition of function with different number of args");
		}

		unsigned int numberArg = 0;
		for(llvm::Function::arg_iterator argIt = function -> arg_begin(); numberArg != args.size(); ++argIt, ++numberArg)
		{
			argIt ->setName(args[numberArg]);
			namedValues[args[numberArg]] = argIt;
		}
		return function;
	}

	llvm::Function* CodeGen::generateFunction(PrototypeAST* prototype, ExpressionAST* body)
	{
		namedValues.clear();
		llvm::Function* function = prototype -> Codegen();
		if(!function) return nullptr;

		llvm::BasicBlock* basicBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", function);
		builder.SetInsertPoint(basicBlock);

		llvm::Value* returnValue = body -> Codegen();
		if(returnValue)
		{
			builder.CreateRet(returnValue);
			llvm::verifyFunction(*function);
			return function;
		}
		function -> eraseFromParent();
		return nullptr;
	}

	llvm::Value* CodeGen::generateImport(std::string name)
	{
		return nullptr;
	}

	llvm::Value* CodeGen::errorV(std::string msg)
	{
		*outputStream << "[CodeGen Value] Error: " << msg << std::endl;
		return nullptr;
	}

	llvm::Function* CodeGen::errorF(std::string msg)
	{
		*outputStream << "[CodeGen Function] Error: " << msg << std::endl;
		return nullptr;
	}
}