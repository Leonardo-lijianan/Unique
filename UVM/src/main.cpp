// Copyright 2022 Source Speace Studio
// License(GPLv3.0)
// Author: Kkasi
// This is main, a VM.

#include <iostream>
#include "fileOperate.h"
#include "bytePrjLoader.h"
#include "executer.h"

using namespace virtualMachine;

int main(int argc, char **argv) {
	/* head */
	printf("Unique VM 0.1.4\n");
	std::string fileName, workDir;
	if(argc==2) {
		workDir = getWorkPath();
		fileName = argv[1];
	} else {
		printf("unique.callexe.CallWayWrong Error:\n"\
			"\tthe call way is wrong!\n"\
			"\ttrue way:\n"\
			"\tque ((proByteCodeName))\n");
		exit(CallWayWrong);
	}

	/* Load byte project */
	BytePrjLoader pler(workDir);
	pler.initFile(fileName);
	printf("[ByteProjectLoader] Loading complete!\n");
	

	/* executing */
	Executer executer(pler.getCodes(),pler.getCsp());
	executer.executing();
	printf("\n[Executer] Executing complete!\n");

	return 0;
}