#include "CoupeCast.h"

#include "../CoupeUtils.h"

namespace Coupe
{
	char* toString(int number)
	{
		return Utils::createCString(lexical_cast<std::string>(number));
	}
	char* toString(double number)
	{
		return Utils::createCString(lexical_cast<std::string>(number));
	}

	int toInteger(double number)
	{
		return static_cast<int>(number);
	}

	int toInteger(char* number)
	{
		return lexical_cast<int>(number);
	}

	double toDouble(int number)
	{
		return static_cast<double>(number);
	}

	double toDouble(char* number)
	{
		return lexical_cast<double>(number);
	}
}