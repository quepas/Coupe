#ifndef COUPE_LIB_MGR_H_
#define COUPE_LIB_MGR_H_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "../CoupeToken.h"
#include <llvm/ExecutionEngine/ExecutionEngine.h>

namespace Coupe
{
	class LibraryMgr
	{	
		// metalib (ML) data
		enum MLType
		{
			MLT_EOF,
			MLT_COMMENT_LINE,
			MLT_IDENTIFIER,
			MLT_INTEGER,
			MLT_DOUBLE,
			MLT_STRING,
			MLT_VOID,
			MLT_LEFT_PARA,
			MLT_RIGHT_PARA,
			MLT_COMMA,
			MLT_ERROR
		};

		struct MLToken
		{
			MLToken(MLType _type, std::string _value)
				: type(_type), value(_value) {}
			std::string value;
			MLType type;
		};

		struct MLData
		{
			std::string name;
			std::vector<Value::Type> argsType;
			Value::Type returnType;
		};

		public:
			void setMetalibFile(std::string filename);
			void setInputStream(std::istream& stream);
			void setOutputStream(std::ostream& stream);
			void beVerbose(bool _verbose);
			void beVerboseAboutToken(MLToken* token);
			void beVerboseAboutData(MLData* data);

			void parseAndExecute(llvm::ExecutionEngine* executionEngine, llvm::Module* module);

			static LibraryMgr& getInstance() 
			{
				static LibraryMgr instance;
				return instance;
			}
		private:
			LibraryMgr() : verbose(false),
						   inputStream(&std::cin),
						   outputStream(&std::cout) {};
			LibraryMgr(const LibraryMgr&) {}
			LibraryMgr& operator=(const LibraryMgr&) {}

			bool verbose;
			std::istream* inputStream;
			std::ostream* outputStream;

			std::vector<MLData*>& parse();
			MLData* parseMLData();
			MLToken* getNextToken();	
			MLToken* getToken();
			char getCharacter();
			void ungetCharacter();	
			Value::Type convertTokenType(MLToken* token);
			llvm::Type* convertValueType(Value::Type type);
			bool isDataType(MLToken* token);
			std::string convertValueTypeToSufix(Value::Type type);
	};
}

#endif