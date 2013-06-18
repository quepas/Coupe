#ifndef COUPE_LIB_CAST_H_
#define COUPE_LIB_CAST_H_

namespace Coupe
{	
	char* toString(int number);
	char* toString(double number);

	int toInteger(double number);
	int toInteger(char* number);

	double toDouble(int number);
	double toDouble(char* number);	
}

#endif