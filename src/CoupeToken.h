#ifndef COUPE_TOKEN_H_
#define COUPE_TOKEN_H_

#include <string>

namespace Coupe
{
	// token related errors

	struct Position
	{
		Position() : row(0), col(0) {}
		Position(int _row, int _col) : row(_row), col(_col) {}

		int row, col;

		void set(int _row, int _col)
		{
			row = _row;
			col = _col;
		}
	};

	struct Value
	{
		enum Type 
		{
			INTEGER,
			FLOAT,
			CHAR,
			STRING
		};
		
		Type type;
		std::string value;					
	};

	enum Type
	{
		TOK_EOF,
		TOK_ERROR,

		TOK_IDENTIFIER,

		TOK_KW_DEF,
		TOK_KW_EXTERN,
		TOK_KW_BEGIN,
		TOK_KW_END,
		TOK_KW_IF,
		TOK_KW_ELSE,
		TOK_KW_LOOP,
		TOK_KW_RETURN,
		TOK_KW_AND,
		TOK_KW_OR,

		TOK_NUMBER,
		TOK_ROUND_LEFT_BRACKET,
		TOK_ROUND_RIGHT_BRACKET,

		TOK_OP_IMPLICATION,
		TOK_OP_MUL,
		TOK_OP_DIV,
		TOK_OP_MOD,
		TOK_OP_SUB,
		TOK_OP_ADD,
		TOK_OP_POWER,

		TOK_COMMENT_LINE,
		TOK_COMMENT_SECTION		
	};

	// for scanner purpose
	struct Token
	{
		Type type;
		Value value;
		Position position;		
	};	
}

#endif