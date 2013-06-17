#ifndef COUPE_LIB_STD_H_
#define COUPE_LIB_STD_H_

#include <stdio.h>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <string>

#include "CoupeString.h"

namespace Coupe
{
	extern "C"
	char* toStringFromFile(char* filename);	
	char* concatenate(char* a, char* b);	
}

#endif

