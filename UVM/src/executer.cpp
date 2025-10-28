// Copyright 2022 Source Speace Studio
// License(GPLv3.0)
// Author: Kkasi
// This is executing the byte code.

#include "executer.h"
#include <unistd.h>

namespace virtualMachine {

Executer::Executer(std::vector<ByteCode> bCodes, std::vector<std::string> constantStringPool)
	: bCodes(bCodes), constantStringPool(constantStringPool), ip(0) {
}

Executer::~Executer() {}

bool Executer::isVstackCode() {
	ByteCode bCode = bCodes.at(ip);
	return bCode.getOpCode()==PUSH || bCode.getOpCode()==POP;
}

bool Executer::isOpCode() {
	ByteCode bCode = bCodes.at(ip);
	return bCode.getOpCode()==ADD || bCode.getOpCode()==SUB ||
	       bCode.getOpCode()==MUL || bCode.getOpCode()==DIV;
}

bool Executer::isVarPoolCode() {
	ByteCode bCode = bCodes.at(ip);
	return bCode.getOpCode()==STORE || bCode.getOpCode()==LOAD;
}

void Executer::visitVstackCode() {
	ByteCode bCode = bCodes.at(ip);
	switch(bCode.getOpCode()) {
		case PUSH: vstack.push(bCode.getOperand()); break;
		case POP:  vstack.pop(); break;
		default: break;
	}
}

bool Executer::isPriCode() {
	return bCodes.at(ip).getOpCode() == opCode::PRI;
}

void Executer::visitOpCode() {
	if(vstack.size()<2) {
		printf("vmStackError: index is out of stack size.");
		exit(-1);
	}
	ByteCode bCode = bCodes.at(ip);
	int op1,op2;
	switch(bCode.getOpCode()) {
		case ADD: op2 = vstack.top(); vstack.pop();
		          op1 = vstack.top(); vstack.pop();
		          vstack.push(op1 + op2); break;
		case SUB: op2 = vstack.top(); vstack.pop();
		          op1 = vstack.top(); vstack.pop();
		          vstack.push(op1 - op2); break;
		case MUL: op2 = vstack.top(); vstack.pop();
		          op1 = vstack.top(); vstack.pop();
		          vstack.push(op1 * op2); break;
		case DIV: op2 = vstack.top(); vstack.pop();
		          op1 = vstack.top(); vstack.pop();
		          vstack.push(op1 / op2); break;
		default: break;
	}
}

void Executer::visitVarPoolCode() {
	ByteCode bCode = bCodes.at(ip);
	switch(bCode.getOpCode()) {
		case STORE: variablePool.push_back(vstack.top()); vstack.pop(); break;
		case LOAD: vstack.push(variablePool.at(bCode.getOperand())); break;
		default: break;
	}
}

void Executer::visitPriCode() {
	ByteCode bCode = bCodes.at(ip);
	if(bCode.getOperand()>=(byte)0xF0) {
		const char* output = constantStringPool.at(bCode.getOperand() & 0x0F).c_str();
		write(STDOUT_FILENO, output, strlen(output));
	}
}

void Executer::executing() {
	printf("\nIO-start>\n");
	ip=0;
	while(bCodes.size()>ip) {
		if(isVstackCode()) {
			visitVstackCode();
		} else if (isOpCode()) {
			visitOpCode();
		} else if (isVarPoolCode()) {
			visitVarPoolCode();
		} else if(isPriCode()) {
			visitPriCode();
		} else {
			return;
		}
		ip++;
	}
	printf("\nIO-end>\n");
	for(int i=0;i<variablePool.size();i++) {
		printf("[Executer] %d : %d\n",i,variablePool.at(i));
	}
}

} // namespace virtualMachine