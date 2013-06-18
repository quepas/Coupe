#include "CoupeLibraryMgr.h"
#include "../CoupeUtils.h"
#include <llvm/DerivedTypes.h>
#include <llvm/LLVMContext.h>
#include <llvm/Function.h>
#include "CoupeStd.h"
#include <map>

namespace Coupe
{
	void LibraryMgr::setInputFile(std::string filename)
	{
		std::ifstream* file = new std::ifstream(filename);
		if(!file -> is_open())
		{
			*outputStream << "Cannot open file: " << filename << std::endl;
			return;
		}
		setInputStream(*file);
	}

	void LibraryMgr::setInputStream(std::istream& stream)
	{
		inputStream = &stream;
	}

	void LibraryMgr::setOutputStream(std::ostream& stream)
	{
		outputStream = &stream;
	}

	void LibraryMgr::beVerbose(bool _verbose)
	{
		verbose = _verbose;
	}

	void LibraryMgr::beVerboseAboutToken(MLToken* token)
	{
		if(verbose)
		{
			*outputStream << "Readed token: +" << token -> value << "+" << std::endl;
		}		
	}

	void LibraryMgr::beVerboseAboutData(MLData* data)
	{
		if(verbose)
		{
			*outputStream << "Created data: " << data -> name << std::endl;
		}		
	}

	LibraryMgr::MLToken* LibraryMgr::getNextToken()
	{
		MLToken* token = getToken();
		beVerboseAboutToken(token);
		return token;
	}

	LibraryMgr::MLToken* LibraryMgr::getToken()
	{		
		std::string value;
		char character;				
		
		do 
		{
			character = getCharacter();				
		} while (Utils::isWhitespace(character));
		value.push_back(character);

		if(Utils::isCharacter(character))
		{
			do 
			{
				character = getCharacter();
				value.push_back(character);
			} while (Utils::isCharacter(character) || Utils::isDigit(character) || character == '_');
			ungetCharacter();
			value.pop_back();

			std::string comparedValue = Utils::toLowerCase(value);

			if(comparedValue == "integer")	return new MLToken(MLT_INTEGER, value);
			if(comparedValue == "double")	return new MLToken(MLT_DOUBLE, value);
			if(comparedValue == "string")	return new MLToken(MLT_STRING, value);
			return new MLToken(MLT_IDENTIFIER, value);
		}
		else
		{
			switch(character)
			{
				case '(':
					return new MLToken(MLT_LEFT_PARA, value);
				case ')':
					return new MLToken(MLT_RIGHT_PARA, value);
				case ',':
					return new MLToken(MLT_COMMA, value);
				case '#':
					do 
					{
						character = getCharacter();
						value.push_back(character);

						if(character == EOF)
						{
							ungetCharacter();
							return new MLToken(MLT_COMMENT_LINE, value);
						}
					} while (character != '\n');
					ungetCharacter();
					value.pop_back();
					return new MLToken(MLT_COMMENT_LINE, value);
				case EOF:
					return new MLToken(MLT_EOF, value);
			}
		}
		return new MLToken(MLT_ERROR, value);
	}	

	void LibraryMgr::parseAndExecute(llvm::ExecutionEngine* executionEngine, llvm::Module* module)
	{
		std::vector<MLData*>& mlDatas = parse();
		std::map<std::string, void*>& function_map = getLibraryFunctionPtr();				
	
		for(unsigned int i = 0; i < mlDatas.size(); ++i)
		{			
			MLData* data = mlDatas[i];			
			std::vector<llvm::Type*> params;
			for(unsigned int j = 0; j < data -> argsType.size(); ++j)
			{
				Value::Type type = data -> argsType[j];
				params.push_back(convertValueType(type));
			}			
			llvm::FunctionType* functionType = llvm::FunctionType::get(convertValueType(data -> returnType), params, false);
			llvm::Function* functionPTR = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, data -> name, module);
			executionEngine -> addGlobalMapping(functionPTR, function_map.at(data -> name));			
		}
	}

	std::vector<LibraryMgr::MLData*>& LibraryMgr::parse()
	{
		std::vector<MLData*>* mlData = new std::vector<MLData*>();
		MLToken* token = getNextToken();

		while(token -> type != MLT_EOF)
		{
			if(isDataType(token))
			{
				Value::Type returnType = convertTokenType(token);
				MLData* data = parseMLData();
				if(data)
				{
					beVerboseAboutData(data);
					data -> returnType = returnType;
					mlData -> push_back(data);
				}
			}
			token = getNextToken();
		}
		return *mlData;
	}

	LibraryMgr::MLData* LibraryMgr::parseMLData()
	{
		MLToken* token = getNextToken();
		MLData* result = new MLData();

		if(token -> type != MLT_IDENTIFIER)
		{
			*outputStream << "expected identifier" << std::endl;
			return nullptr;
		}
		result -> name = token -> value;

		token = getNextToken();
		if(token -> type != MLT_LEFT_PARA)
		{
			*outputStream << "expected left paranthesis \'(\'" << std::endl;
			return nullptr;
		}

		do
		{
			token = getNextToken();

			if(isDataType(token))
			{
				result -> argsType.push_back(convertTokenType(token));
				token = getNextToken();

				switch(token -> type)					
				{
					case MLT_RIGHT_PARA:
						return result;				
					case MLT_COMMA:				
						*outputStream << "expected \',\'" << std::endl;
						return nullptr;
				}
			}
			else 
			{
				*outputStream << "expected argument type" << std::endl;
				return nullptr;
			}
		} while (true);
	}

	Value::Type LibraryMgr::convertTokenType(MLToken* token)
	{
		switch(token -> type)
		{
			case MLT_INTEGER:
				return Value::INTEGER;
			case MLT_DOUBLE:
				return Value::DOUBLE;
			case MLT_STRING:
				return Value::STRING;
		}
	}

	llvm::Type* LibraryMgr::convertValueType(Value::Type type)
	{
		switch(type)
		{
			case Value::INTEGER:
				return llvm::IntegerType::getInt32Ty(llvm::getGlobalContext());
			case Value::DOUBLE:		
				return llvm::Type::getPrimitiveType(llvm::getGlobalContext(), llvm::Type::DoubleTyID);
			case Value::STRING:
				return llvm::IntegerType::getInt8PtrTy(llvm::getGlobalContext());
		}
		return nullptr;		
	}

	bool LibraryMgr::isDataType(MLToken* token)
	{
		switch(token -> type)
		{
			case MLT_INTEGER:
			case MLT_DOUBLE:
			case MLT_STRING:
				return true;
			default:
				return false;
		}
	}

	char LibraryMgr::getCharacter()
	{
		return inputStream -> get();
	}

	void LibraryMgr::ungetCharacter()
	{
		inputStream -> unget();
	}	
}