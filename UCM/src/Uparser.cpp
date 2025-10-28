// Copyright 2022 Source Speace Studio
// License(GPLv3.0)
// Author: Kkasi
// This is turning tokens to AST.

#include <string>
#include "Uparser.h"


namespace compiler {

Parser::Parser()
	: pos(0), prog(nullptr),parser_state(RUNNING) {
}

Parser::~Parser() {
}

void Parser::throwSyntaxWrong(std::string exceptedStr) {
	if(pos>=tokenList_Main.getSize()) pos=pos-1;
	printf("unique.compiler.parser.SyntaxWrong Error:\n"               \
	"\tIn %d:%d, excpted %s, before token'%s'\n"                       \
	,tokenList_Main.getToken(getPos()).getLine()                       \
	,tokenList_Main.getToken(getPos()).getRow()                        \
	,exceptedStr.c_str()                                               \
	,tokenList_Main.getToken(getPos()).getData().c_str()               );
	exit(SyntaxWrong);
}

bool Parser::isNumTerminal() {
	if(parser_state==ENDING) {
		//throwSyntaxWrong("OOR(find num terminal)");
		return false;
	}
	return tokenList_Main.getToken(getPos()).getType()==T_NUM;
}

bool Parser::isStrTerminal() {
	if(parser_state==ENDING) {
		//throwSyntaxWrong("OOR(find num terminal)");
		return false;
	}
	return tokenList_Main.getToken(getPos()).getType()==T_STR;
}

bool Parser::isIdnTerminal() {
	if(parser_state==ENDING) {
		//throwSyntaxWrong("OOR(find idn terminal)");
		return false;
	}
	return tokenList_Main.getToken(getPos()).getType()==T_WORD;
}

bool Parser::isTermOp() {
	if(parser_state==ENDING) {
		//printf("OOR");
		//throwSyntaxWrong("OOR(find term op)");
		return false;
	}
	return (tokenList_Main.getToken(getPos()).getType()==T_MUL || tokenList_Main.getToken(getPos()).getType()==T_DIV);
}

bool Parser::isExprOp() {
	if(parser_state==ENDING) {
		//throwSyntaxWrong("OOR(find exp op)");
		return false;
	}
	return (tokenList_Main.getToken(getPos()).getType()==T_ADD || tokenList_Main.getToken(getPos()).getType()==T_SUB);
}

bool Parser::isLeftPth() {
	if(parser_state==ENDING) {
		//throwSyntaxWrong("OOR(find left pth)");
		return false;
	}
	return tokenList_Main.getToken(getPos()).getType()==T_PTHL;
}

bool Parser::isRightPth() {
	if(parser_state==ENDING) {
		//throwSyntaxWrong("OOR(find right pth)");
		return false;
	}
	return tokenList_Main.getToken(getPos()).getType()==T_PTHR;
}

bool Parser::isAssignment() {
	if(parser_state==ENDING) {
		//throwSyntaxWrong("OOR(find ass)");
		return false;
	}
	return tokenList_Main.getToken(getPos()).getType()==T_ASS;
}

bool Parser::isNegativeStart() {
	if(parser_state==ENDING) {
		//throwSyntaxWrong("OOR(find neg)");
		return false;
	}
	return tokenList_Main.getToken(getPos()).getType()==T_SUB;
}

bool Parser::isFactorStart() {
	return (isNumTerminal() || isIdnTerminal() || isLeftPth() || isNegativeStart());
}

bool Parser::isTermStart() {
	return isFactorStart();
}

bool Parser::isExprStart() {
	return isTermStart();
}

bool Parser::isAssignStatStart() {
	return isIdnTerminal();
}

bool Parser::isPrintStatStart() {
	if(parser_state==ENDING) {
		//throwSyntaxWrong("OOR(find print func)");
		return false;
	}
	return tokenList_Main.getToken(getPos()).getType()==T_KEYWORD && tokenList_Main.getToken(getPos()).getInfo() == keyword::PRINT;
}

bool Parser::isProgamStart() {
	return isAssignStatStart() || isPrintStatStart();
}

void Parser::next() {
	pos = pos + 1;
	if(pos>=tokenList_Main.getSize()) {
		//printf("next OOR! pos[%d]\n",pos);
		parser_state = ENDING;
	}
}

Terminal_Pointer Parser::getPos() {
	if(pos>=tokenList_Main.getSize()) {
		printf("get OOR! pos[%d]\n",pos);
		parser_state = ENDING;
		return -1;
	}
	return pos;
}

Terminal_Pointer Parser::number() {
	if(!isNumTerminal()) {
		throwSyntaxWrong("couldn't find num");
		return 0x0;
	}
	return getPos();
}

Terminal_Pointer Parser::String() {
	if(!isStrTerminal()) {
		throwSyntaxWrong("couldn't find num");
		return 0x0;
	}
	return getPos();
}

Terminal_Pointer Parser::identifier() {
	if(!isIdnTerminal()) {
		throwSyntaxWrong("couldn't find idn");
		return 0x0;
	}
	return getPos();
}

Terminal_Pointer Parser::termOp() {
	if(!isTermOp()) {
		return 0x0;
	}
	return getPos();
}

Terminal_Pointer Parser::exprOp() {
	if(!isExprOp()) {
		return 0x0;
	}
	return getPos();
}

NegativeNode *Parser::negative() {
	if(!isNegativeStart()) {return 0x0;}
	next();
	NegativeNode *negt = new NegativeNode;
	negt->setNumber(number());
	next();
	return negt;
}

// factor -> PTHL expr PTHR | NUM | IDN | negative
FactorNode *Parser::factor() {
	if(!isFactorStart()) {
		return 0x0; // [ParserError]factor error.
	} else {
		FactorNode *fac=new FactorNode;
		if(isNumTerminal()) {
			fac->setOperand(number()); next();
		} else if(isIdnTerminal()) {
			fac->setOperand(identifier()); next();
		} else if(isNegativeStart()) {
			fac->setNegtFactor(negative());
		} else if(isLeftPth()) {
			next();// skip "("
			if(isExprStart()) {
				fac->setExprFactor(expr());
			} else {
				delete fac;
				throwSyntaxWrong("{factor} '(' | number | word | neg-number");
				return 0x0;
			}
			if(isRightPth()) {
				next(); // skip ")"
			} else {
				delete fac;
				throwSyntaxWrong("')'");
				return 0x0;
			}
		}
		return fac;
	}
}

// term -> factor ((MUL|DIV) factor)*
TermNode *Parser::term() {
	if(!isTermStart()) {
		throwSyntaxWrong(" {term} '(' | number | word | neg-number");
		return 0x0;
	} else {
		TermNode *ter = new TermNode;
		ter->addFactor(factor());
		while(isTermOp()) {
			ter->addOperator(termOp());
			next();
			ter->addFactor(factor());
		}
		return ter;
	}
}

// expr -> term ((ADD|SUB) term)*
ExprNode *Parser::expr() {
	if(!isExprStart()) {
		throwSyntaxWrong(" {expr} '(' | number | word | neg-number");
		return 0x0;
	} else {
		ExprNode *expr = new ExprNode;
		expr->addFactor(term());
		while(isExprOp()) {
			expr->addOperator(exprOp());
			next();
			expr->addFactor(term());		
		}
		return expr;
	}
}

// assign -> IDN ASS expr
AssignStatNode *Parser::assignStat() {
	if(!isAssignStatStart()) {
		throwSyntaxWrong("a word");
		return 0x0;
	} else {
		AssignStatNode *ass = new AssignStatNode;
		ass->setIdentifier(identifier()); next();
		if(isAssignment()) {
			next(); // skip "="
		} else {
			throwSyntaxWrong("'='");
			return 0x0;
		}
		ass->setFactor(expr());
		return ass;
	}
}


// statExpr -> assign+ | print+
// StatExprNode *Parser::statExpr() {
// 	StatExprNode *prog = new StatExprNode;
// 	while (isStatStart()) {
// 		if(isPrintFuncStart()) {
// 			printf("isPrint!");
// 			exit(-100);
// 		} else {
// 			while (isAssignStart()) {
// 				prog->addFactor(assign());
// 			}
// 		}
// 	}
// 	if(parser_state!=ENDING) {
// 		throwSyntaxWrong("a word");
// 		return 0x0;
// 	} else if(isPrintFuncStart()) {
// 		printf("isPrint!");
// 		exit(-100);
// 	} else {
// 		StatExprNode *prog = new StatExprNode;
// 		while (isAssignStart()) {
// 			prog->addFactor(assign());
// 		}
// 		return prog;
// 	}
// }

PrintStatNode* Parser::printStat() {
	next(); // skip the "print"
	if(parser_state==ENDING) {
		throwSyntaxWrong("a word");
		return 0x0;
	} else {
		PrintStatNode* pri = new PrintStatNode;
		if(this->isLeftPth()) {
			next();
			pri->setString(String());
			next();
			if(this->isRightPth()) {
				next();
			} else {
				throwSyntaxWrong("a right pth ')'");
				return 0x0;
			}
		} else {
			throwSyntaxWrong("a left pth '('");
			return 0x0;
		}
		return pri;
	}
}

ProgramNode* Parser::program() {
	printf("programNode\n");
	ProgramNode* prog = new ProgramNode;
	while (isProgamStart()) {
		if(isPrintStatStart()) {
			printf("isPrintStatStart\n");
			prog->addStatement(printStat());
		} else /* if(isAssignStatStart()) */ {
			prog->addStatement(assignStat());
		}
	}
	return prog;
}

int Parser::parsing() {
	prog=program();
	return 0;
}

void Parser::showAST() {
	if(prog) {
		prog->show();
	}
}

ProgramNode *Parser::getAST() const {
	return this->prog;
}


} // namespace compiler