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
	 *	ExpressionAST
	 */
	class ExpressionAST
	{
		public:
			virtual ~ExpressionAST() {}
	};

	/**
	 *	NumberAST -> ExpressionAST
	 */
	class NumberAST : public ExpressionAST
	{
		public:
			NumberAST(int value);
			NumberAST(float value);
			NumberAST(double value);		

		private:
			NumberValue number;				
	};

	class VariableAST : public ExpressionAST
	{
		public:
			VariableAST(std::string _name) : name(_name) {};

		private:
			std::string name;
	};

	/**
	 * BinaryOpAST -> ExpressionAST (for binary operators)
	 */
	class BinaryOpAST : public ExpressionAST
	{
		public:
			BinaryOpAST(char _op, ExpressionAST* _LHS, ExpressionAST* _RHS)
				: op(_op), LHS(_LHS), RHS(_RHS) {}

		private:
			char op; // TODO: maybe a token info here? :)
			ExpressionAST *LHS, *RHS;
	};

	class CallAST : public ExpressionAST
	{
		public:
			CallAST(std::string _callee, const std::vector<ExpressionAST*>& _args)
				: callee(_callee), args(_args) {}
		private:
			std::string callee;
			std::vector<ExpressionAST*> args;
	};

	class PrototypeAST
	{
		public:
			PrototypeAST(std::string _name, const std::vector<std::string>& _args)
				: name(_name), args(_args) {}
		private:
			std::string name;
			std::vector<std::string> args;
	};

	class FunctionAST
	{
		public:
			FunctionAST(PrototypeAST* _prototype, ExpressionAST* _body)
				: prototype(_prototype), body(_body) {}
		private:
			PrototypeAST* prototype;
			ExpressionAST* body;
	};

	class ImportAST
	{
		public: 
			ImportAST(std::string _name) : name(_name) {}
		private:
			std::string name;
	};
}

#endif