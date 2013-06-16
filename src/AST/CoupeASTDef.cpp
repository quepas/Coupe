#include "CoupeASTDef.h"

#include "../CoupeCodeGen.h"
#include "../CoupeUtils.h"

namespace Coupe
{
	NumberAST::NumberAST(int value)
	{
		number.value.i = value;
		number.type = NumberValue::INTEGER;
	}

	NumberAST::NumberAST(double value)
	{
		number.value.d = value;
		number.type = NumberValue::DOUBLE;
	}
	llvm::Value* NumberAST::Codegen()
	{		
		return CodeGen::getInstance().generateNumber(number);
	}
	llvm::Value* StringAST::Codegen(){
		return CodeGen::getInstance().generateString(text);
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

	llvm::Function* PrototypeAST::Codegen()
	{
		return CodeGen::getInstance().generatePrototype(name, args, returnType);
	}
	
	llvm::Function* FunctionAST::Codegen()
	{
		return CodeGen::getInstance().generateFunction(prototype, body);
	}	
}