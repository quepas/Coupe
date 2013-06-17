#ifndef DEFINITIONS_AST_H_
#define DEFINITIONS_AST_H_

#include <llvm\Value.h>
#include <llvm\Function.h>

#include "../CoupeToken.h"

#include <string>
#include <vector>

namespace Coupe
{	
	struct NumberValue 
	{
		union UnionNumber
		{
			int i;			
			double d;
		};

		enum Type
		{
			INTEGER,			
			DOUBLE
		};

		UnionNumber value;
		Type type;
	};
	
	class ExpressionAST
	{
		public:
			virtual ~ExpressionAST() {}
			virtual llvm::Value* Codegen() = 0;
	};

	class NumberAST : public ExpressionAST
	{
		public:
			NumberAST(int value);	
			NumberAST(double value);		
			virtual llvm::Value* Codegen();

		private:
			NumberValue number;				
	};

	class StringAST : public ExpressionAST
	{
		public:
			StringAST(std::string _text) : text(_text) {};
			virtual llvm::Value* Codegen();

		private:
			std::string text;
	};

	class VariableAST : public ExpressionAST
	{
		public:
			VariableAST(std::string _name) : name(_name) {};
			virtual llvm::Value* Codegen();			

		private:
			std::string name;
	};
	
	class BinaryOpAST : public ExpressionAST
	{
		public:
			BinaryOpAST(Type _op, ExpressionAST* _LHS, ExpressionAST* _RHS)
				: op(_op), LHS(_LHS), RHS(_RHS) {}
			virtual llvm::Value* Codegen();

		private:
			Type op;
			ExpressionAST *LHS, *RHS;
	};

	class ImportAST
	{
		public: 
			ImportAST(std::string _name) : name(_name) {}
			virtual llvm::Value* Codegen();

			std::string getName() { return name; }
		private:
			std::string name;
	};

	class CallAST : public ExpressionAST
	{
		public:
			CallAST(std::string _callee, const std::vector<ExpressionAST*>& _args)
				: callee(_callee), args(_args) {}
			virtual llvm::Value* Codegen();

			std::string getCallee() { return callee; }
			std::vector<ExpressionAST*>& getArgs() { return args; }
		private:
			std::string callee;
			std::vector<ExpressionAST*> args;
	};

	class PrototypeAST
	{
		public:
			PrototypeAST(std::string _name, const std::vector<std::string>& _args, llvm::Type* _returnType = nullptr)
				: name(_name), args(_args), returnType(_returnType) {}
			virtual llvm::Function* Codegen();

			std::string getName() { return name; }
			std::vector<std::string>& getArgs() { return args; }
		private:
			std::string name;
			std::vector<std::string> args;
			llvm::Type* returnType;
	};

	class FunctionAST
	{
		public:
			FunctionAST(PrototypeAST* _prototype, ExpressionAST* _body)
				: prototype(_prototype), body(_body) {}
			virtual llvm::Function* Codegen();

			PrototypeAST* getPrototype() { return prototype; }
			ExpressionAST* getBody() { return body; }
		private:
			PrototypeAST* prototype;
			ExpressionAST* body;
	};	
}

#endif