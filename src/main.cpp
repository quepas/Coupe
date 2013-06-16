#include <iostream>
#include <string>

#include "CoupeScanner.h"
#include "CoupeParser.h"
#include "CoupeToken.h"

#include "llvm/Analysis/Passes.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/TargetSelect.h"


int main()
{
	std::string fileName = "test_files/coupe1.txt";
	std::string fileNameFib = "test_files/coupe_fib.txt";

	  llvm::InitializeNativeTarget();
  //LLVMContext &Context = getGlobalContext();

	Coupe::Parser parser;
	parser.beVerbose(true);
	//parser.setInputFile(fileName);
	// tu bylem: mpanek

	  // Create the JIT.  This takes ownership of the module.
  std::string ErrStr;
  parser.TheExecutionEngine = llvm::EngineBuilder(parser.codeGen.mainModule).setErrorStr(&ErrStr).create();
  if (!parser.TheExecutionEngine) {
    fprintf(stderr, "Could not create ExecutionEngine: %s\n", ErrStr.c_str());
    exit(1);
  }

  llvm::FunctionPassManager OurFPM(parser.codeGen.mainModule);

  // Set up the optimizer pipeline.  Start with registering info about how the
  // target lays out data structures.
  OurFPM.add(new llvm::DataLayout(*(parser.TheExecutionEngine)->getDataLayout()));
  // Provide basic AliasAnalysis support for GVN.
  OurFPM.add(llvm::createBasicAliasAnalysisPass());
  // Do simple "peephole" optimizations and bit-twiddling optzns.
  OurFPM.add(llvm::createInstructionCombiningPass());
  // Reassociate expressions.
  OurFPM.add(llvm::createReassociatePass());
  // Eliminate Common SubExpressions.
  OurFPM.add(llvm::createGVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  OurFPM.add(llvm::createCFGSimplificationPass());

  OurFPM.doInitialization();

  // Set the global so the code gen can use this.
  parser.codeGen.TheFPM = &OurFPM;

	parser.parse();

	getchar();
}