#ifndef COUPE_LIB_FILE_H_
#define COUPE_LIB_FILE_H_

namespace Coupe
{
	// Pattern: open file and read
	char* file(char* filename); 
	// Pattern: open file and write
	char* fileW(char* filename, char* content);
	// Pattern: open file and append
	char* fileAppend(char* filename, char* content);
}

#endif