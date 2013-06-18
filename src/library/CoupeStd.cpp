#include "CoupeStd.h"


namespace Coupe 
{
	std::map<std::string, void*>& getLibraryFunctionPtr()
	{
		static std::map<std::string, void*> functionsPtr = std::map<std::string, void*>();		

		// Cast Library
		functionsPtr["toStringI"] = (char* (*)(int)) &toStringI;
		functionsPtr["toStringD"] = (char* (*)(double)) &toStringD;
		functionsPtr["toIntegerD"] = (int (*)(double)) &toIntegerD;
		functionsPtr["toIntegerS"] = (int (*)(char*)) &toIntegerS;
		functionsPtr["toDoubleI"] = (double (*)(int)) &toDoubleI;
		functionsPtr["toDoubleS"] = (double (*)(char*)) &toDoubleS;

		// File Library
		functionsPtr["file"] = (char* (*)(char*)) &file;

		// String Library
		functionsPtr["lower"] = (char* (*)(char*)) &lower;
		functionsPtr["upper"] = (char* (*)(char*)) &upper;
		functionsPtr["concat"] = (char* (*)(char*, char*)) &concat;
		functionsPtr["split1"] = (char* (*)(char*, char*)) &split1;
		functionsPtr["split2"] = (char* (*)(char*, char*)) &split2;
		functionsPtr["tagsH"] = (char* (*)(char*, char*)) &tagsH;
		functionsPtr["tags"] = (char* (*)(char*, char*, char*)) &tags;

		// Math Library
		functionsPtr["sqrt"] = (double (*)(double)) &sqrt;

		// Std Library
		functionsPtr["empty"] = (double (*)()) &empty;
		functionsPtr["identity"] = (double (*)(double)) &identity;

		return functionsPtr;
	}

	double empty()
	{
		return 0.0;
	}

	double identity(double number)
	{
		return number;
	}
}

