#include "CoupeCast.h"

#include "../CoupeUtils.h"

namespace Coupe
{
	char* toStringI(int number)
	{
		return Utils::createCString(lexical_cast<std::string>(number));
	}
	char* toStringD(double number)
	{
		return Utils::createCString(lexical_cast<std::string>(number));
	}

	int toIntegerD(double number)
	{
		return static_cast<int>(number);
	}

	int toIntegerS(char* number)
	{
		return lexical_cast<int>(number);
	}

	double toDoubleI(int number)
	{
		return static_cast<double>(number);
	}

	double toDoubleS(char* number)
	{
		return lexical_cast<double>(number);
	}
}