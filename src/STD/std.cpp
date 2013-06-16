#include "std.h";

char* toStringFromFile(char* filename){
	std::ifstream file = std::ifstream(filename);
	std::string content, line;

	while(std::getline(file, line))
	{	
		content.append(line);
	}

	char* str = (char*)malloc(sizeof(content.size()*sizeof(char)));
	strcpy(str, content.c_str());

	return str;
}

char* toLower(char* c){
	char* z = c;
	int i;
	for(i=0; z[i] != NULL; i++)
		z[i] = tolower(z[i]);
	return z;
}

char* toUpper(char* c){
	char* z = c;
	int i;
	for(i=0; z[i] != NULL; i++)
		z[i] = toupper(z[i]);
	return z;
}

char* concatenate(char* a, char* b){
	std::string aa(a);
	std::string bb(b);
	std::string cc;
	cc = aa+bb;
	char* str = (char*)malloc(sizeof(cc.size()*sizeof(char)));
	strcpy(str,cc.c_str());
	return str;
}

int stringToInt(char* a){
	return atoi(a);
}

double stringToDouble(char* a){
	return atof(a);
}

char* intToString(int a){
	char* aa = (char*)malloc(10*sizeof(char));
	itoa(a,aa,10);
	return aa;
}