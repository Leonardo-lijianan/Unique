// Copyright 2022 Source Speace Studio
// License(GPLv3.0)
// Author: Kkasi
// This is executing the byte code.

#ifndef UNIQUE_UVM_EXECUTER_H_
#define UNIQUE_UVM_EXECUTER_H_

#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include "def.h"
#include "byteCode.h"

namespace virtualMachine {

class Executer {
private:
	std::vector<ByteCode> bCodes;
	std::stack<int> vstack;
	std::vector<int> variablePool;
	std::vector<std::string> constantStringPool;
	usint ip;
protected:
	bool isVstackCode();
	bool isOpCode();
	bool isVarPoolCode();
	bool isPriCode();
	
	void visitVstackCode();
	void visitOpCode();
	void visitVarPoolCode();
	void visitPriCode();
public:
	Executer(std::vector<ByteCode> bCodes, std::vector<std::string> constantStringPool);
	~Executer();
	void executing();
};

} // namespace virtualMachine

#endif // UNIQUE_UVM_EXECUTER_H_