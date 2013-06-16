#include <stdio.h>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <string>

extern "C"
char* toStringFromFile(char* filename);
char* toLower(char* c);
char* toUpper(char* c);
char* concatenate(char* a, char* b);
int stringToInt(char* a);
double stringToDouble(char* b);
char* intToString(int a);