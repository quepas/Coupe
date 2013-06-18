#ifndef COUPE_LIB_STRING_H_
#define COUPE_LIB_STRING_H_

namespace Coupe
{
	char* lower(char* text);
	char* upper(char* text);
	char* concat(char* text1, char* text2);
	// Pattern: split and return first half
	char* split1(char* text, char* delimeter);
	// Pattern: split and return second half
	char* split2(char* text, char* delimeter);
	// Pattern: get content from html tags (without '<', '>')
	char* tagsH(char* text, char* tag);
	// Pattern: get content between tags
	char* tags(char* text, char* beginTag, char* endTag);
}

#endif