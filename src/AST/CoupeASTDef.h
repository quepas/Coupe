#ifndef DEFINITIONS_AST_H_
#define DEFINITIONS_AST_H_

#include <string>
#include <vector>

namespace Coupe
{
	struct NumberValue 
	{
		union UnionNumber
		{
			int i;
			float f;
			double d;
		};

		enum Type
		{
			INTEGER,
			FLOAT,
			DOUBLE
		};

		UnionNumber value;
		Type type;
	};
	
	/**
	 *	BaseAST
	 */
	class BaseAST
	{
		public:
			virtual ~BaseAST() {}
	};

	/**
	 *	NumberAST -> BaseAST
	 */
	class NumberAST : public BaseAST
	{
		public:
			NumberAST(int value);
			NumberAST(float value);
			NumberAST(double value);		

		private:
			NumberValue number;				
	};

	class VariableAST : public BaseAST
	{
		public:
			VariableAST(std::string _name) : name(_name) {};

		private:
			std::string name;
	};

	/**
	 * BinaryOpAST -> BaseAST (for binary operators)
	 */
	class BinaryOpAST : public BaseAST
	{
		public:
			BinaryOpAST(char _op, BaseAST* _LHS, BaseAST* _RHS)
				: op(_op), LHS(_LHS), RHS(_RHS) {}

		private:
			char op; // TODO: maybe a token info here? :)
			BaseAST *LHS, *RHS;
	};

	class CallAST : public BaseAST
	{
		public:
			CallAST(std::string _callee, const std::vector<BaseAST*>& _args)
				: callee(_callee), args(_args) {}
		private:
			std::string callee;
			std::vector<BaseAST*> args;
	};

	class PrototypeAST : public BaseAST
	{
		public:
			PrototypeAST(std::string _name, const std::vector<std::string>& _args)
				: name(_name), args(_args) {}
		private:
			std::string name;
			std::vector<std::string> args;
	};

	class FunctionAST : public BaseAST
	{
		public:
			FunctionAST(PrototypeAST* _prototype, BaseAST* _body)
				: prototype(_prototype), body(_body) {}
		private:
			PrototypeAST* prototype;
			BaseAST* body;
	};
}

#endif