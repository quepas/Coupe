#ifndef COUPE_TOKEN_H_
#define COUPE_TOKEN_H_

#include <string>

namespace Coupe
{	
	struct Position
	{
		Position() : row(0), col(0) {}
		Position(int _row, int _col) : row(_row), col(_col) {}

		unsigned int row, col;

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
			DOUBLE,
			CHAR,
			STRING
		};
		
		Type type;
		std::string data;					
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
		TOK_KW_IMPORT,

		TOK_INTEGER,
		TOK_DOUBLE,
		TOK_STRING,
		TOK_ROUND_LEFT_BRACKET,
		TOK_ROUND_RIGHT_BRACKET,

		TOK_OP_IMPLICATION,
		TOK_OP_MUL,
		TOK_OP_DIV,
		TOK_OP_MOD,
		TOK_OP_SUB,
		TOK_OP_ADD,
		TOK_OP_POWER,
		
		TOK_OP_LESS,
		TOK_OP_MORE,		

		TOK_COMMENT_LINE,
		TOK_COMMENT_SECTION,

		TOK_COMMA,
		TOK_SEMICOLON,
		TOK_COLON,
		TOK_KW_PATTERN
	};	

	struct Token
	{
		Type type;
		Value value;
		Position position;		
	};	
}

#endif