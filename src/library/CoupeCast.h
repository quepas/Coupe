#ifndef COUPE_LIB_CAST_H_
#define COUPE_LIB_CAST_H_

namespace Coupe
{	
	char* toStringI(int number);
	char* toStringD(double number);

	int toIntegerD(double number);
	int toIntegerS(char* number);

	double toDoubleI(int number);
	double toDoubleS(char* number);	
}

#endif