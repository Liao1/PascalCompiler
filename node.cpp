#include "node.h"
using namespace std;
using namespace llvm;

Value* ConstAST::Codegen(CodeGenContext& astcontext)  //by ly
{
	cout<<"const"<<endl;
	Value *constVal, *alloc;

	if (type->type == Integer) {
		NumberExprAST *n = new NumberExprAST(value.i);
		// n->type = Number;
		constVal = n->Codegen(astcontext);//生成对应常量
	} else if (type->type == Real) {
		RealExprAST *d = new RealExprAST(value.d);
		// r.type = Real;
		constVal = d->Codegen(astcontext);
	} else if (type->type == Bool) {
		BoolExprAST *b = new BoolExprAST(value.b);
		// b.type = Bool;
		constVal = b->Codegen(astcontext);
	} else if (type->type == String) {
		StringExprAST *s = new StringExprAST(value.s);
		// s.type = String;
		constVal = s->Codegen(astcontext);
	} else if (type->type == Char) {
		CharExprAST *c = new CharExprAST(value.c);
		// c.type = Char;
		constVal = c->Codegen(astcontext);
	}
	std::vector<string> v;
	v.push_back(variableName);
	VariableDeclAST *variabledecl = new VariableDeclAST(type, v);
	variabledecl->Codegen(astcontext);//定义对应类型的变量
	return builder.CreateStore(constVal, astcontext.locals.find(variableName)->second);//对变量进行赋值
}
/*Value* SelfdefineTypeAST::Codegen(CodeGenContext& context)	//by ly
{
	return NULL;
}
Value* RecordTypeAST::Codegen(CodeGenContext& context)	//by ly
{
	return NULL;
}
Value* ChangeNameTypeAST::Codegen(CodeGenContext& context)	//by ly
{
	return NULL;
}*/
Value* VariableDeclAST::Codegen(CodeGenContext& astcontext)
{
	cout<<"var"<<endl;
	Value *alloc;
	for(int i = 0; i < variableName.size(); i++)
	{
		if (astcontext.locals.count(variableName[i])!=0){
			return NULL;
		}
		if (type->type == Integer)
		{
			cout<<"new integer"<<endl;
			Value *val = builder.getInt32(0);
			alloc = new llvm::GlobalVariable(module, Type::getInt32Ty(llvm::getGlobalContext()), false, GlobalValue::ExternalLinkage, builder.getInt32(0));
			// alloc = builder.CreateAlloca(Type::getInt32Ty(llvm::getGlobalContext()));
			// cout<<"create store"<<endl;
			// cout << alloc << endl;
			builder.CreateStore(val, alloc);
			cout<<"sign up at varTable"<<endl;	
			cout << variableName[i] << endl;
			// astcontext.locals.find(this->variableName[i])->second = alloc;
			astcontext.locals[variableName[i]] = alloc;
		}
		else if (type->type == Real)
		{
			cout<<"new real"<<endl;
			Value *val = ConstantFP::get(Type::getDoubleTy(getGlobalContext()), 0.0);
			alloc = new llvm::GlobalVariable(module, Type::getDoubleTy(llvm::getGlobalContext()), false, GlobalValue::ExternalLinkage, ConstantFP::get(Type::getDoubleTy(getGlobalContext()), 0.0));
			// alloc = builder.CreateAlloca(Type::getInt32Ty(llvm::getGlobalContext()));
			// cout<<"create store"<<endl;
			// cout << alloc << endl;
			builder.CreateStore(val, alloc);
			cout<<"sign up at varTable"<<endl;	
			cout << variableName[i] << endl;
			// astcontext.locals.find(this->variableName[i])->second = alloc;
			astcontext.locals[variableName[i]] = alloc;
		}
		else if (type->type == Bool)
		{
			cout<<"new bool"<<endl;
			alloc = new llvm::AllocaInst(Type::getInt32Ty(llvm::getGlobalContext()), this->variableName[i].c_str());
		   	// builder.CreateStore(NULL, alloc);			
			astcontext.locals[variableName[i]] = alloc;
		} else {
			return NULL;
		}
	}
	return alloc;
}

//表达式Codegen
//整数常数表达式，如“1”
Value* NumberExprAST::Codegen(CodeGenContext& astcontext)
{
	std::cout << "Creating integer: " << val << std::endl;
	return ConstantInt::get(Type::getInt32Ty(getGlobalContext()), val, true);
}
//实数常数表达式，如“1.0”
Value* RealExprAST::Codegen(CodeGenContext& astcontext)
{
	std::cout << "Creating double: " << val << std::endl;
	return ConstantFP::get(Type::getDoubleTy(getGlobalContext()), val);
}
Value* BoolExprAST::Codegen(CodeGenContext& astcontext)
{
	std::cout << "Creating bool: " << val << std::endl;
	return ConstantInt::get(Type::getInt64Ty(getGlobalContext()), val, true);
}
Value* StringExprAST::Codegen(CodeGenContext& astcontext)	//by ly
{
	cout << "Creating string: " << val << endl;
	return builder.CreateGlobalStringPtr(val);
}
Value* CharExprAST::Codegen(CodeGenContext& astcontext)	//by ly
{
	cout << "Create char: " << val << endl;
	string s = "";
	s = s + val;
	return builder.CreateGlobalStringPtr(s);
}
Value* VariableExprAST::Codegen(CodeGenContext& astcontext)
{  
  // Look this variable up in the function.
  Value *V = astcontext.locals.find(name)->second;
  cout<<"pick variable:"<<name<<endl;
  return builder.CreateLoad(V);
}
/*Value* ArrayVariableExprAST::Codegen(CodeGenContext& context)
{
	return NULL;
}
Value* RecordVariableExprAST::Codegen(CodeGenContext& context)
{
	return NULL;
}*/
Value* UnaryExprAST::Codegen(CodeGenContext& astcontext)
{
  //   if(op == '-'){	
  //   	float tmp = -(expr->val);
		// return ConstantFP::get(Type::getDoubleTy(getGlobalContext()), tmp);
  //   }
  //   else 
	return NULL;
}
Value* BinaryExprAST::Codegen(CodeGenContext& astcontext)
{
	Value *L, *R;
	if (op!=assignmentKind)
		L = LExpr->Codegen(astcontext);
	R = RExpr->Codegen(astcontext);
	// if (L == 0 || R == 0) return 0;
	std::cout << "Creating binary operation " << op << std::endl;
	/*if (op == plusKind)
	{
		return builder.CreateFAdd(L, R, "addtmp");	}
	else if (op == minusKind)
	{
		return builder.CreateFSub(L, R, "subtmp");
	}
	else if (op == orKind)
	{
		return NULL;
	}
	else if (op == mulKind)
	{
		return builder.CreateFMul(L, R, "multmp");
	}
	else if (op == divKind)
	{
		return builder.CreateSDiv(L, R, "divtmp");	
	}
	else if (op == modKind)
	{
		return NULL;
	}
	else if (op == andKind)
	{
		return NULL;
	}
	else if (op == geKind)
	{
		return NULL;
	}
	else if (op == gtKind)
	{
		return NULL;
	}
	else if (op == leKind)
	{
		return NULL;
	}
	else if (op == ltKind)
	{
		L = builder.CreateFCmpULT(L, R, "cmptmp");
		// Convert bool 0/1 to double 0.0 or 1.0
		return builder.CreateUIToFP(L, Type::getDoubleTy(getGlobalContext()),"booltmp");

	}
	else if (op == eqKind)
	{
		return NULL;
	}
	else if (op == ueqKind)
	{
		return NULL;
	}
	else if (op == assignmentKind)
	{
		if (context.locals().find(LExpr->name) == context.locals().end()) {
			throw std::domain_error("Undeclared variable " + LExpr->name);
			return nullptr;
		}
		return builder.CreateStore(RExpr->Codegen(context),context.locals()[LExpr->name]);
	}
	return NULL;*/
	switch (op){
		case plusKind: return builder.CreateAdd(L, R, "addtmp");
		case minusKind: return builder.CreateSub(L, R, "subtmp");
		case assignmentKind: 
		{
			Value *v = astcontext.locals.find((dynamic_cast<VariableExprAST *>(LExpr))->name)->second;
			builder.CreateStore(R, v);
			return NULL;
		}
		case ltKind:
		{
			L = builder.CreateFCmpULT(L, R, "cmptmp");
			// Convert bool 0/1 to double 0.0 or 1.0
			return builder.CreateUIToFP(L, Type::getDoubleTy(getGlobalContext()),"booltmp");

		}
		default: return NULL;
	}
}
Value* CallFunctionExprAST::Codegen(CodeGenContext& astcontext)
{
	cout<<"call"<<endl;
	Function *CalleeF = module.getFunction(callee);
	if (CalleeF == 0)
		// return ErrorV("Unknown function referenced");
		return NULL;
	// If argument mismatch error.
	if (CalleeF->arg_size() != args.size())
		// return ErrorV("Incorrect # arguments passed");
		return NULL;
	std::vector<Value*> ArgsV;
	for (unsigned i = 0, e = args.size(); i != e; ++i) {
		ArgsV.push_back(args[i]->Codegen(astcontext));
		if (ArgsV.back() == 0) return 0;
	}
	ArrayRef<Value*> args(ArgsV);
	return builder.CreateCall(CalleeF, args, "calltmp");
}

Value* CallProcedureExprAST::Codegen(CodeGenContext& astcontext)
{
	cout<<"call"<<endl;
	Function *CalleeF = module.getFunction(callee);
	if (CalleeF == 0)
		// return ErrorV("Unknown function referenced");
		return NULL;
	// If argument mismatch error.
	if (CalleeF->arg_size() != args.size())
		// return ErrorV("Incorrect # arguments passed");
		return NULL;
	std::vector<Value*> ArgsV;
	for (unsigned i = 0, e = args.size(); i != e; ++i) {
		ArgsV.push_back(args[i]->Codegen(astcontext));
		if (ArgsV.back() == 0) return 0;
	}
	ArrayRef<Value*> args(ArgsV);
	return builder.CreateCall(CalleeF, args);
}

Value* IfExprAST::Codegen(CodeGenContext& astcontext)
{
	  Value *CondV = ifCond->Codegen(astcontext);
	  if (CondV == 0)
		return 0;

	  // Convert condition to a bool by comparing equal to 0.0.
	  CondV = builder.CreateFCmpONE(
		  CondV, ConstantFP::get(getGlobalContext(), APFloat(0.0)), "ifcond");

	  Function *TheFunction = builder.GetInsertBlock()->getParent();

	  // Create blocks for the then and else cases.  Insert the 'then' block at the
	  // end of the function.
	  BasicBlock *ThenBB =
		  BasicBlock::Create(getGlobalContext(), "then", TheFunction);
	  BasicBlock *ElseBB = BasicBlock::Create(getGlobalContext(), "else");
	  BasicBlock *MergeBB = BasicBlock::Create(getGlobalContext(), "ifcont");

	  builder.CreateCondBr(CondV, ThenBB, ElseBB);

	  // Emit then value.
	  builder.SetInsertPoint(ThenBB);

	  vector<Value *> ThenV;
	  for (int i = 0; i < thenComponent.size(); i++)
	  ThenV.push_back(thenComponent[i]->Codegen(astcontext));

	  builder.CreateBr(MergeBB);
	  // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
	  ThenBB = builder.GetInsertBlock();

	  // Emit else block.
	  TheFunction->getBasicBlockList().push_back(ElseBB);
	  builder.SetInsertPoint(ElseBB);
	  vector<Value *> ElseV;
	  for (int i = 0; i < elseComponent.size(); i++)
	  ElseV.push_back(elseComponent[i]->Codegen(astcontext));

	  builder.CreateBr(MergeBB);
	  // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
	  ElseBB = builder.GetInsertBlock();

	  // Emit merge block.
	  TheFunction->getBasicBlockList().push_back(MergeBB);
	  builder.SetInsertPoint(MergeBB);
	  PHINode *PN =
		  builder.CreatePHI(Type::getDoubleTy(getGlobalContext()), 2, "iftmp");
	  for (int i = 0; i < ThenV.size(); i++)
	  PN->addIncoming(ThenV[i], ThenBB);
	  for (int i = 0; i < ElseV.size(); i++)
	  PN->addIncoming(ElseV[i], ElseBB);
	  return PN;
}

/*
Value* ForExprAST::Codegen(CodeGenContext& context)
{
	  Value *StartVal = start->Codegen(context);
	  if (StartVal == 0)
		return 0;

	  // Make the new basic block for the loop header, inserting after current
	  // block.
	  Function *TheFunction = builder.GetInsertBlock()->getParent();
	  BasicBlock *PreheaderBB = builder.GetInsertBlock();
	  BasicBlock *LoopBB =
		  BasicBlock::Create(getGlobalContext(), "loop", TheFunction);

	  // Insert an explicit fall through from the current block to the LoopBB.
	  builder.CreateBr(LoopBB);

	  // Start insertion in LoopBB.
	  builder.SetInsertPoint(LoopBB);

	  // Start the PHI node with an entry for Start.
	  PHINode *Variable = builder.CreatePHI(Type::getDoubleTy(getGlobalContext()),
											2, varName.c_str());
	  Variable->addIncoming(StartVal, PreheaderBB);

	  // Within the loop, the variable is defined equal to the PHI node.  If it
	  // shadows an existing variable, we have to restore it, so save it now.
	  Value *OldVal = NamedValues[varName];
	  NamedValues[varName] = Variable;

	  // Emit the body of the loop.  This, like any other expr, can change the
	  // current BB.  Note that we ignore the value computed by the body, but don't
	  // allow an error.
	  if (body->Codegen(context) == 0)
		return 0;

	  // Emit the step value.
	  Value *StepVal;
	  StepVal = ConstantFP::get(getGlobalContext(), APFloat(1.0));
	  Value *NextVar = builder.CreateFAdd(Variable, StepVal, "nextvar");

	  // Compute the end condition.
	  Value *EndCond = end->Codegen(context);
	  if (EndCond == 0)
		return EndCond;

	  // Convert condition to a bool by comparing equal to 0.0.
	  EndCond = builder.CreateFCmpONE(
		  EndCond, ConstantFP::get(getGlobalContext(), APFloat(0.0)), "loopcond");

	  // Create the "after loop" block and insert it.
	  BasicBlock *LoopEndBB = builder.GetInsertBlock();
	  BasicBlock *AfterBB =
		  BasicBlock::Create(getGlobalContext(), "afterloop", TheFunction);

	  // Insert the conditional branch into the end of LoopEndBB.
	  builder.CreateCondBr(EndCond, LoopBB, AfterBB);

	  // Any new code will be inserted in AfterBB.
	  builder.SetInsertPoint(AfterBB);

	  // Add a new entry to the PHI node for the backedge.
	  Variable->addIncoming(NextVar, LoopEndBB);

	  // Restore the unshadowed variable.
	  if (OldVal)
		NamedValues[VarName] = OldVal;
	  else
		NamedValues.erase(VarName);

	  // for expr always returns 0.0.
	  return Constant::getNullValue(Type::getDoubleTy(getGlobalContext()));

}*/
/*Value* WhileExprAST::Codegen(CodeGenContext& context)
{
	return NULL;
}
Value* RepeatExprAST::Codegen(CodeGenContext& context)
{
	return NULL;
}*/
Value* FunctionAST::Codegen(CodeGenContext& astcontext)
{
	cout<<"function"<<endl;
	std::vector<llvm::Type*> arg_types;
	for(int i = 0;i < headerDecl.size();i++)
	{
		cout<<"args"<<endl;
		for (int j=0; j<headerDecl[i]->variableName.size(); j++)
			if (headerDecl[i]->type->type == Integer)
				arg_types.push_back(llvm::Type::getInt32Ty(llvm::getGlobalContext())); 
			else
				arg_types.push_back(llvm::Type::getVoidTy(llvm::getGlobalContext())); 
	}
	FunctionType *f_type;
	cout<<"create f_type"<<endl;
	if (returnType && returnType->type == Integer)
		f_type = llvm::FunctionType::get(this->isProcedure() ? llvm::Type::getVoidTy(llvm::getGlobalContext()) : llvm::Type::getInt32Ty(llvm::getGlobalContext()), llvm::makeArrayRef(arg_types), false);
	else
		f_type = llvm::FunctionType::get(this->isProcedure() ? llvm::Type::getVoidTy(llvm::getGlobalContext()) : llvm::Type::getVoidTy(llvm::getGlobalContext()), llvm::makeArrayRef(arg_types), false);
	cout<<"create function"<<endl;
	Function *function = llvm::Function::Create(f_type, llvm::GlobalValue::InternalLinkage, this->name.c_str(), &module);
	// auto block = llvm::BasicBlock::Create(getGlobalContext(), "entry", function, 0);
	builder.SetInsertPoint(BasicBlock::Create(context,"entry",function));
	// push block and start routine
	// context.pushBlock(block);

	// deal with arguments
	llvm::Value* arg_value;	
	cout<<"create args_values"<<endl;
	auto args_values = function->arg_begin();
	for (int i = 0; i < headerDecl.size(); i++)
	{
		cout<<"create code for headerDecl"<<endl;
		headerDecl[i]->Codegen(astcontext);
		cout<<"set argValue"<<endl;
		for (int j=0; j<headerDecl[i]->variableName.size(); j++){
			cout<<"args_values++"<<endl;
			arg_value = args_values++;
			cout<<"set Name"<<endl;
			arg_value->setName(headerDecl[i]->variableName[j].c_str());
			cout<<"Create store"<<endl;
			builder.CreateStore(arg_value,astcontext.locals.find(headerDecl[i]->variableName[j])->second);
		}
		// auto inst = new llvm::StoreInst(arg_value, context.locals()[arg->name->name], false, block);

	}

	// add function return variable
	if (this->isFunction()) {
		std::cout << "Creating function return value declaration" << std::endl;
		Value *alloc;
		if (this->returnType->type == Integer) 
			alloc = new llvm::AllocaInst(Type::getInt32Ty(llvm::getGlobalContext()), this->name.c_str());
		else
			alloc = new llvm::AllocaInst(Type::getVoidTy(llvm::getGlobalContext()), this->name.c_str());
		// builder.CreateStore(NULL, alloc);		
		astcontext.locals[this->name] = alloc;	
		// auto alloc = new llvm::AllocaInst(this->return_type->toLLVMType(), this->routine_name->name.c_str(), context.currentBlock());
	}
	// deal with constant define
	for (int i=0; i<consts.size(); i++){
		consts[i]->Codegen(astcontext);
	}
	// deal with variable declaration
	for (int i = 0; i < bodyDecl.size(); i++)
	{
		bodyDecl[i]->Codegen(astcontext);
	}
	// deal with program statements
	for (int i = 0; i < body.size(); i++)
	{
		cout<<"stmt"<<endl;
		body[i]->Codegen(astcontext);
	}

	// return value
	if (this->isFunction()) {
		std::cout << "Generating return value for function" << std::endl;
		auto load_ret = new llvm::LoadInst(astcontext.locals.find(this->name)->second, this->name);
	   // llvm::ReturnInst::Create(llvm::getGlobalContext(), load_ret, block);
		builder.CreateRet(load_ret);
	} else {
		std::cout << "Generating return void for procedure" << std::endl;
		builder.CreateRetVoid();
	 //   llvm::ReturnInst::Create(llvm::getGlobalContext(), block);
	}

	for (int i = 0; i < functions.size(); i++)
	{
		functions[i]->Codegen(astcontext);
	}
	// pop block and finsh
  //  context.popBlock();
  //  std::cout << "Creating " << this->toString() << ":" << this->routine_name->name << std::endl;
	return function;

}
