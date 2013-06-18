#include "CoupeString.h"

#include "../CoupeUtils.h"

namespace Coupe
{
	char* lower(char* text)
	{
		return Utils::createCString(Utils::toLowerCase(std::string(text)));
	}

	char* upper(char* text)
	{
		return Utils::createCString(Utils::toUpperCase(std::string(text)));
	}

	char* concat(char* _text1, char* _text2)
	{
		std::string text1(_text1), text2(_text2);
		return Utils::createCString(text1+text2);
	}
	// Pattern: split and return first half
	char* split1(char* _text, char* _delimeter)
	{
		std::string text(_text), delimeter(_delimeter);
		std::string::size_type index = text.find(delimeter);

		if(index != std::string::npos)
		{
			return Utils::createCString(text.substr(0, index));
		}
		return _text;
	}

	// Pattern: split and return second half
	char* split2(char* _text, char* _delimeter)
	{
		std::string text(_text), delimeter(_delimeter);
		std::string::size_type index = text.find(delimeter);

		if(index != std::string::npos)
		{
			return Utils::createCString(text.substr(index+delimeter.length()));
		}
		return _text;
	}

	// Pattern: get content from html tags (without '<', '>')
	char* tagsH(char* text, char* _tag)
	{
		std::string tag(_tag), 
					beginTag = "<" + tag + ">",
					endTag = "</" + tag + ">";
		return tags(text, Utils::createCString(beginTag), Utils::createCString(endTag));
	}
	// Pattern: get content between tags
	char* tags(char* _text, char* _beginTag, char* _endTag)
	{
		std::string text(_text), 
					beginTag(_beginTag), 
					endTag(_endTag);
		std::string::size_type beginIndex = text.find(beginTag), 
							   endIndex = text.find(endTag, beginIndex);

		if(beginIndex != std::string::npos)
		{
			if(endIndex != std::string::npos)
			{
				return Utils::createCString(text.substr(beginIndex+beginTag.length(), 
					endIndex-beginIndex-beginTag.length()));
			}
			else
			{
				return Utils::createCString(text.substr(beginIndex+beginTag.length()));
			}
		}
		return _text;
	}

}