#include <node.h>
#include "GenIR.h"
using namespace std;


using namespace llvm;

Value* ConstAST::Codegen(CodeGenContext& context)  //by ly
{
	string variableName;
	BasicTypeAST* type;
	constValue value;

}
Value* SelfdefineTypeAST::Codegen(CodeGenContext& context)	//by ly
{

}
Value* RecordTypeAST::Codegen(CodeGenContext& context)	//by ly
{

}
Value* ChangeNameTypeAST::Codegen(CodeGenContext& context)	//by ly
{

}
Value* VariableDeclAST::Codegen(CodeGenContext& context)
{
		for(int i = 0; i < variableName.size(); i++)
	{
        if (type->type == INTEGER)
        {
			auto alloc = new llvm::AllocaInst(Type::getInt32Ty(llvm::getGlobalContext()), this->variableName[i].c_str(), context.currentBlock());
		    Builder.CreateStore(this->variableName[i].c_str(), alloc);        	
		    context.locals()[this->variableName[i]] = alloc;	
        }
        else if (type->type == REAL)
        {
			auto alloc = new llvm::AllocaInst(Type::getDoubleTy(llvm::getGlobalContext()), this->variableName[i].c_str(), context.currentBlock());
		    Builder.CreateStore(this->variableName[i].c_str(), alloc);        	
		    context.locals()[this->variableName[i]] = alloc;	
        }
        else if (type->type == BOOL)
        {
			auto alloc = new llvm::AllocaInst(Type::getInt32Ty(llvm::getGlobalContext()), this->variableName[i].c_str(), context.currentBlock());
		   	Builder.CreateStore(this->variableName[i].c_str(), alloc);        	
		    context.locals()[this->variableName[i]] = alloc;	
        }	
	}
    return alloc;

}

//表达式Codegen
//整数常数表达式，如“1”
Value* NumberExprAST::Codegen(CodeGenContext& context)
{
	std::cout << "Creating integer: " << val << std::endl;
	return ConstantInt::get(Type::getInt64Ty(getGlobalContext()), val, true);
}
//实数常数表达式，如“1.0”
Value* RealExprAST::Codegen(CodeGenContext& context)
{
	std::cout << "Creating double: " << val << std::endl;
	return ConstantFP::get(Type::getDoubleTy(getGlobalContext()), val);
}
Value* BoolExprAST::Codegen(CodeGenContext& context)
{
	std::cout << "Creating bool: " << val << std::endl;
	return ConstantInt::get(Type::getInt64Ty(getGlobalContext()), val, true);
}
Value* StringExprAST::Codegen(CodeGenContext& context)	//by ly
{
}
Value* CharExprAST::Codegen(CodeGenContext& context)	//by ly
{
}
Value* VariableExprAST::Codegen(CodeGenContext& context)
{  
}
Value* ArrayVariableExprAST::Codegen(CodeGenContext& context)
{
}
Value* RecordVariableExprAST::Codegen(CodeGenContext& context)
{
}
Value* UnaryExprAST::Codegen(CodeGenContext& context)
{
  //   if(op == '-'){	
  //   	float tmp = -(expr->val);
		// return ConstantFP::get(Type::getDoubleTy(getGlobalContext()), tmp);
  //   }
  //   else 
  //   	return null;
}
Value* BinaryExprAST::Codegen(CodeGenContext& context)
{
	  Value *L = LExpr->Codegen();
	  Value *R = RExpr->Codegen();
	  if (L == 0 || R == 0) return 0;
	  case '+': return builder.CreateFAdd(L, R, "addtmp");
	  case '-': return builder.CreateFSub(L, R, "subtmp");
	  case '*': return builder.CreateFMul(L, R, "multmp");
	  case '<':
	    L = builder.CreateFCmpULT(L, R, "cmptmp");
	    // Convert bool 0/1 to double 0.0 or 1.0
	    return builder.CreateUIToFP(L, Type::getDoubleTy(getGlobalContext()),"booltmp");
	  }

	std::cout << "Creating binary operation " << op << std::endl;
	if (op == plus_kind)
	{
		return builder.CreateFAdd(L, R, "addtmp");	}
	else if (op == minus_kind)
	{
		return builder.CreateFSub(L, R, "subtmp");
	}
	else if (op == or_kind)
	{

	}
	else if (op == mul_kind)
	{
		return builder.CreateFMul(L, R, "multmp");
	}
	else if (op == div_kind)
	{
		return builder.CreateSDiv(L, R, "divtmp");	
	}
	else if (op == mod_kind)
	{

	}
	else if (op == and_kind)
	{

	}
	else if (op == ge_kind)
	{

	}
	else if (op == gt_kind)
	{

	}
	else if (op == le_kind)
	{

	}
	else if (op == lt_kind)
	{
	    L = builder.CreateFCmpULT(L, R, "cmptmp");
	    // Convert bool 0/1 to double 0.0 or 1.0
	    return builder.CreateUIToFP(L, Type::getDoubleTy(getGlobalContext()),"booltmp");

	}
	else if (op == eq_kind)
	{

	}
	else if (op == ueq_kind)
	{

	}
	else if (op == assignment_kind)
	{
	    if (context.locals().find(LExpr->name) == context.locals().end()) {
	        throw std::domain_error("Undeclared variable " + LExpr->name);
	        return nullptr;
	    }
	    return builder.CreateStore(RExpr->Codegen(context),context.locals()[LExpr->name]);
	}
	return NULL;

}
Value* CallExprAST::Codegen(CodeGenContext& context)
{
	  Function *CalleeF = module->getFunction(callee);
	  if (CalleeF == 0)
	    return ErrorV("Unknown function referenced");
	  
	  // If argument mismatch error.
	  if (CalleeF->arg_size() != args.size())
	    return ErrorV("Incorrect # arguments passed");

	  std::vector<Value*> ArgsV;
	  for (unsigned i = 0, e = args.size(); i != e; ++i) {
	    ArgsV.push_back(args[i]->Codegen(context));
	    if (ArgsV.back() == 0) return 0;
	  }
	  
	  return builder.CreateCall(CalleeF, ArgsV, "calltmp");

}
Value* IfExprAST::Codegen(CodeGenContext& context)
{
	  Value *CondV = ifCond->Codegen(context);
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

	  Value *ThenV = thenComponent->Codegen(context);
	  if (ThenV == 0)
	    return 0;

	  builder.CreateBr(MergeBB);
	  // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
	  ThenBB = builder.GetInsertBlock();

	  // Emit else block.
	  TheFunction->getBasicBlockList().push_back(ElseBB);
	  builder.SetInsertPoint(ElseBB);

	  Value *ElseV = elseComponent->Codegen(context);
	  if (ElseV == 0)
	    return 0;

	  builder.CreateBr(MergeBB);
	  // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
	  ElseBB = builder.GetInsertBlock();

	  // Emit merge block.
	  TheFunction->getBasicBlockList().push_back(MergeBB);
	  builder.SetInsertPoint(MergeBB);
	  PHINode *PN =
	      builder.CreatePHI(Type::getDoubleTy(getGlobalContext()), 2, "iftmp");

	  PN->addIncoming(ThenV, ThenBB);
	  PN->addIncoming(ElseV, ElseBB);
	  return PN;

}
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

}
Value* WhileExprAST::Codegen(CodeGenContext& context)
{
}
Value* RepeatExprAST::Codegen(CodeGenContext& context)
{
}
Function* PrototypeAST::Codegen(CodeGenContext& context)
{
	std::vector<Type*> Doubles(Args.size(),
                             Type::getDoubleTy(getGlobalContext()));
    FunctionType *FT = FunctionType::get(Type::getDoubleTy(getGlobalContext()),
                                       Doubles, false);
    
    Function *F = Function::Create(FT, Function::ExternalLinkage, Name, module);
  
    // If F conflicted, there was already something named 'Name'.  If it has a
    // body, don't allow redefinition or reextern.
    if (F->getName() != Name) 
    {
    // Delete the one we just made and get the existing one.
    F->eraseFromParent();
    F = module->getFunction(Name);
    
    // If F already has a body, reject this.
    if (!F->empty()) 
    {
      ErrorF("redefinition of function");
      return 0;
    }
    
    // If F took a different number of args, reject.
    if (F->arg_size() != Args.size()) 
    {
      ErrorF("redefinition of function with different # args");
      return 0;
    }
  }
  
  // Set names for all arguments.
  unsigned Idx = 0;
  for (Function::arg_iterator AI = F->arg_begin(); Idx != Args.size();
       ++AI, ++Idx) {
    AI->setName(Args[Idx]);
    
    // Add arguments to variable symbol table.
    context.locals[Args[Idx]] = AI;
  }
  
  return F;

}
Function* FunctionAST::Codegen(CodeGenContext& context)
{
	  context.locals.clear();
  
	  Function *TheFunction = proto->Codegen();
	  if (TheFunction == 0)
	    return 0;
	  
	  // Create a new basic block to start insertion into.
	  BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", TheFunction);
	  builder.SetInsertPoint(BB);
	  
	  if (Value *RetVal = body->Codegen()) {
	    // Finish off the function.
	    builder.CreateRet(RetVal);

	    // Validate the generated code, checking for consistency.
	    verifyFunction(*TheFunction);

	    return TheFunction;
	  }
	  
	  // Error reading body, remove function.
	  TheFunction->eraseFromParent();
	  return 0;

}
Value* ProgramAST::Codegen(CodeGenContext& context)
{
	
	Value* last = nullptr;    
	for(int i = 0; i < consts.size())) {
        last = records[i]->Codegen(context);
    }
    for(int i = 0; i < records.size())) {
        last = records[i]->Codegen(context);
    }
    for(int i = 0; i < decl.size()) {
        last = decl[i]->Codegen(context);
    }
    for(int i = 0; i < functions.size()) {
        last = functions[i]->Codegen(context);
    }
    return last;

}