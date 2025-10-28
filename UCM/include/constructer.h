// Copyright 2022 Source Speace Studio
// License(GPLv3.0)
// Author: Kkasi
// This can turn AST into byteCode.

#ifndef UNIQUE_UCM_CONSTRUCTER_
#define UNIQUE_UCM_CONSTRUCTER_

#include <string>
#include <vector>
#include <map>
#include "ASTNode.h"
#include "byteCode.h"

namespace compiler {

class Constructer {
private:
	ProgramNode *prog;
	std::map<std::string,usint> variableTable;
	std::vector<std::string> constantStringPool;
	usint idCur;
	std::vector<byte> byteCode;
protected:
	void makeByteCode(opCode op, int operand);
	void visitTermOp(Terminal_Pointer terl);
	void visitExprOp(Terminal_Pointer terl);
	void visitNegativeNode(NegativeNode *negt);
	void visitFactorNode(FactorNode *fac);
	void visitTermNode(TermNode *ter);
	void visitExprNode(ExprNode *expr);

	void visitAssignStatNode(AssignStatNode *ass);
	void visitPrintStatNode(PrintStatNode *pri);
	void visitProgramNode(ProgramNode* prog);
public:
	Constructer();
	Constructer(ProgramNode* prog);
	~Constructer();
	void constructing();
	std::vector<byte> getCode() const;
	std::vector<std::string> getCsp() const;
	void showByteCode();
};

} // namespace compiler

#endif // UNIQUE_UCM_CONSTRUCTER_