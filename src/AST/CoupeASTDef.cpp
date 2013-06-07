#include "CoupeASTDef.h"

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
}