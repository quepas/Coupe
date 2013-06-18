#include "CoupeStd.h"

std::map<std::string, void*>& Coupe::getLibraryFunctionPtr()
{
	std::map<std::string, void*>* functionsPtr = new std::map<std::string, void*>();		

	// Library Cast
	(*functionsPtr)["toString"] = (char* (*)(int)) &toString;
	(*functionsPtr)["toString"] = (char* (*)(double)) &toString;
	(*functionsPtr)["toInteger"] = (int (*)(double)) &toInteger;
	(*functionsPtr)["toInteger"] = (int (*)(char*)) &toInteger;
	(*functionsPtr)["toDouble"] = (double (*)(int)) &toDouble;
	(*functionsPtr)["toDouble"] = (double (*)(char*)) &toDouble;

	// Library File
	(*functionsPtr)["file"] = (char* (*)(char*)) &file;

	// Library String
	(*functionsPtr)["lower"] = (char* (*)(char*)) &lower;
	(*functionsPtr)["upper"] = (char* (*)(char*)) &upper;
	(*functionsPtr)["concat"] = (char* (*)(char*, char*)) &concat;
	(*functionsPtr)["split1"] = (char* (*)(char*, char*)) &split1;
	(*functionsPtr)["split2"] = (char* (*)(char*, char*)) &split2;
	(*functionsPtr)["tags"] = (char* (*)(char*, char*)) &tags;
	(*functionsPtr)["tags"] = (char* (*)(char*, char*, char*)) &tags;

	return *functionsPtr;
}