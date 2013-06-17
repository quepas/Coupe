#include "CoupeFile.h"

namespace Coupe
{
	// Pattern: open file and read
	void file(char* filename)
	{
		std::ifstream file = std::ifstream(filename);
		std::string content, line;

		while(std::getline(file, line))
			content.append(line);	
		file.close();
		return Utils::createCString(content);		
	}

	// Pattern: open file and write
	void file(char* filename, char* _content)
	{
		std::ofstream file = std::ofstream(filename, std::ios_base::trunc);
		std::string content(_content);
		file << content;
		file.close();
	}
	// Pattern: open file and append
	void fileAppend(char* filename, char* content)
	{
		std::ofstream file = std::ofstream(filename, std::ios_base::ate);
		std::string content(_content);
		file << content;
		file.close();
	}
}