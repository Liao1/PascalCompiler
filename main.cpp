#include <iostream>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/ToolOutputFile.h>
#include <llvm/Support/FormattedStream.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/PassManager.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/CodeGen/CommandFlags.h>
using namespace std;

#include "SyntaxTree.h"
#include "SymbolTable.h"
#include "TypeChecking.h"
#include "MiddleWare.h"
#include "node.h"
#include "lex.yy.c"

extern void createSystemFunctions(CodeGenContext& context);
LLVMContext &context = getGlobalContext();
Module module("test",context);
IRBuilder<> builder(context);
Function *startFunc = NULL;
string errorMsg;
FunctionAST *program = NULL;

int main(int argc,char **argv){
	bool runJit = false;
	bool irOutput = true;
	bool asmOutput = true;
	bool objOutput = true;
	TargetMachine::CodeGenFileType outputFileType = TargetMachine::CGFT_Null;
	char *outputFileName = NULL;
	int option;
	
	/* ================================================= */
	yyin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	lineNo = 1;
	yyparse();
	BuildSymTab();
	TypeChecking(root);
	//fprintf(fout, "root");
	//if (!root) printf("null\n");
	//print_tree(root, 1);
	program = CreateFunctionAST(root);
	// program->print(0);
	/* ================================================= */

	CodeGenContext astContext;
	//astContext.addType("long",builder.getInt64Ty());
	//astContext.addType("double",builder.getDoubleTy());
	//astContext.addType("bool",builder.getInt1Ty());
	createSystemFunctions(astContext);


	program->Codegen(astContext);
	//module.dump();cout<<endl;

	InitializeNativeTarget();
	InitializeAllTargets();
	InitializeAllTargetMCs();
	InitializeAllAsmPrinters();
	InitializeAllAsmParsers();

	char *inputFileName = NULL;
	if(irOutput){
		string opFileName;
		if(outputFileName == NULL){
			if(inputFileName == NULL){
				opFileName = "temp.ir";
			}else{
				opFileName = string(basename(inputFileName)) + ".ir";
			}
		}else{
			opFileName = outputFileName;
		}
		string errorMsg;
		tool_output_file outputFile(opFileName.c_str(),errorMsg);
		if(!errorMsg.empty()){
			cout<<errorMsg<<endl;
			return 1;
		}
		outputFile.os()<<module;
		outputFile.keep();
	}

	if(outputFileType != TargetMachine::CGFT_Null){
		Triple triple(sys::getDefaultTargetTriple());
		string errorMsg;
		const Target *target = TargetRegistry::lookupTarget(MArch,triple,errorMsg);
		if(target == NULL){
			cout<<errorMsg<<endl;
			return 1;
		}
		TargetOptions targetOptions;
		TargetMachine *targetMachine = 
				target->createTargetMachine(triple.getTriple(),MCPU,"",targetOptions);

		string opFileName;
		if(outputFileName == NULL){
			if(inputFileName == NULL){
				if(asmOutput){
					opFileName = "temp.s";
				}else{
					opFileName = "temp.o";
				}
			}else{
				if(asmOutput){
					opFileName = string(basename(inputFileName)) + ".s";
				}else{
					opFileName = string(basename(inputFileName)) + ".o";
				}
			}
		}else{
			opFileName = outputFileName;
		}
		string errorMsg2;
		tool_output_file *outputFile = new tool_output_file(opFileName.c_str(),errorMsg2);
		if(!errorMsg2.empty()){
			cout<<errorMsg2<<endl;
			return 1;
		}
		PassManager passManager;
		passManager.add(new DataLayout(&module));
		formatted_raw_ostream fos(outputFile->os());
		targetMachine->addPassesToEmitFile(passManager,fos,outputFileType);
		passManager.run(module);
		outputFile->keep();
	}

	if(runJit){
		string errStr;
		ExecutionEngine *execEngine = EngineBuilder(&module).setErrorStr(&errStr).setEngineKind(EngineKind::JIT).create();  
		if(execEngine == NULL){
			cout<<"Could not create ExecutionEngine: "<<errStr<<endl;
			exit(1);
		}
		vector<GenericValue> argValues;
		execEngine->runFunction(startFunc,argValues);
	}

	return 0;
}

void createSystemFunctions(CodeGenContext& astContext){
	//insert printf func decl
	cout << "1" << endl; 
	vector<Type*> printfFuncArgTypes;
	printfFuncArgTypes.push_back(builder.getInt8PtrTy());
	ArrayRef<Type*> printfFuncArgTypesRef(printfFuncArgTypes);
	FunctionType *printfFuncType = FunctionType::get(builder.getInt32Ty(),printfFuncArgTypesRef,true);
	Constant *printfFunc = module.getOrInsertFunction("printf",printfFuncType);

	vector<Type*> emptyTypes;

	//create print long func
	cout << "2" << endl; 
	vector<Type*> printfLongFuncArgTypes;
	printfLongFuncArgTypes.push_back(builder.getInt32Ty());
	ArrayRef<Type*> printfLongFuncArgTypesRef(printfLongFuncArgTypes);
	FunctionType *printfLongFuncType = FunctionType::get(builder.getVoidTy(),printfLongFuncArgTypesRef,false);
	Function *printfLongFunc = Function::Create(printfLongFuncType,Function::ExternalLinkage,"write",&module);
	builder.SetInsertPoint(BasicBlock::Create(context,"entry",printfLongFunc));
	Value *longFormat = builder.CreateGlobalStringPtr("%d");
	builder.CreateCall2(printfFunc,longFormat,printfLongFunc->arg_begin());
	builder.CreateRetVoid();
	//FunctionAST *printfL = new FunctionAST("write",printfLongFunc,emptyTypes,printfLongFuncArgTypes);

	//create print double func
	/*cout << "3" << endl;
	vector<Type*> printfDoubleFuncArgTypes;
	printfDoubleFuncArgTypes.push_back(builder.getDoubleTy());
	ArrayRef<Type*> printfDoubleFuncArgTypesRef(printfDoubleFuncArgTypes);
	FunctionType *printfDoubleFuncType = FunctionType::get(builder.getVoidTy(),printfDoubleFuncArgTypesRef,false);
	Function *printfDoubleFunc = Function::Create(printfDoubleFuncType,Function::ExternalLinkage,"printD",&module);
	builder.SetInsertPoint(BasicBlock::Create(context,"entry",printfDoubleFunc));
	Value *doubleFormat = builder.CreateGlobalStringPtr("%lf");
	builder.CreateCall2(printfFunc,doubleFormat,printfDoubleFunc->arg_begin());
	builder.CreateRetVoid();*/
	//FunctionAST *printfD = new FunctionAST("printD",printfDoubleFunc,emptyTypes,printfDoubleFuncArgTypes);

	//create println func
	/*cout << "4" << endl;
	FunctionType *printlnFuncType = FunctionType::get(builder.getVoidTy(),false);
	Function *printlnFunc = Function::Create(printlnFuncType,Function::ExternalLinkage,"println",&module);
	builder.SetInsertPoint(BasicBlock::Create(context,"entry",printlnFunc));
	Value *lnFormat = builder.CreateGlobalStringPtr("\n");
	builder.CreateCall(printfFunc,lnFormat);
	builder.CreateRetVoid();*/
	//FunctionAST *println = new FunctionAST("println",printlnFunc,emptyTypes,emptyTypes);

	//astContext.addFunction("printf",cast<Function>(printfFunc));
	//astContext.addFunction("write",printfL);
	//astContext.addFunction("printD",printfD);
	//astContext.addFunction("println",println);
}