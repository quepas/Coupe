#ifndef COUPE_LIB_STD_H_
#define COUPE_LIB_STD_H_

#include "CoupeCast.h"
#include "CoupeFile.h"
#include "CoupeString.h" 
#include "CoupeMath.h"

#include <map>
#include <string>

namespace Coupe
{
	std::map<std::string, void*>& getLibraryFunctionPtr();

	double empty();
	double identity(double number);
}

#endif