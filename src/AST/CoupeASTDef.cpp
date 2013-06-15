#include "CoupeASTDef.h"
#include "../CoupeCodeGen.h"
#include "../CoupeUtils.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/DerivedTypes.h"

namespace Coupe
{
	NumberAST::NumberAST(int value)
	{
		number.value.i = value;
		number.type = NumberValue::INTEGER;
	}

	NumberAST::NumberAST(float value)
	{
		number.value.f = value;
		number.type = NumberValue::FLOAT;
	}

	NumberAST::NumberAST(double value)
	{
		number.value.d = value;
		number.type = NumberValue::DOUBLE;
	}

	llvm::Value* NumberAST::Codegen()
	{		
		return CodeGen::getInstance().generateNumber(number.value.d);
	}

	llvm::Value* VariableAST::Codegen()
	{
		return CodeGen::getInstance().generateVariable(name);
	}

	llvm::Value* BinaryOpAST::Codegen()
	{
		return CodeGen::getInstance().generateBinaryOp(op, LHS, RHS);
	}

	llvm::Value* ImportAST::Codegen()
	{
		return CodeGen::getInstance().generateImport(name);
	}

	llvm::Value* CallAST::Codegen()
	{
		return CodeGen::getInstance().generateCall(callee, args);
	}

	llvm::Value* PrototypeAST::Codegen()
	{
		return CodeGen::getInstance().generatePrototype(name, args);
	}
	
	llvm::Value* FunctionAST::Codegen()
	{
		return CodeGen::getInstance().generateFunction(prototype, body);
	}	
}