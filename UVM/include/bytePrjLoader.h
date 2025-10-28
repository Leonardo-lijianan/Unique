// Copyright 2022 Source Speace Studio
// License(GPLv3.0)
// Author: Kkasi
// This is load byte code project to executing.

#ifndef UNIQUE_UVM_BYTEPRJLOADER_H_
#define UNIQUE_UVM_BYTEPRJLOADER_H_

#include <string>
#include <vector>
#include <fstream>
#include "def.h"
#include "byteCode.h"

namespace virtualMachine {

class BytePrjLoader {
private:
	std::string workDir;
	std::vector<ByteCode> bCodes;
	std::vector<std::string> constantStringPool;
public:
	BytePrjLoader(std::string workDir);
	~BytePrjLoader();
	bool isQueFlie(std::ifstream& inf);
	void readContantStringPool(std::ifstream& inf);
	void readCode(std::ifstream& inf);
	void initFile(std::string fileName);
	std::vector<ByteCode> getCodes() const;
	std::vector<std::string> getCsp() const;
};

} // namespace virtualMachine

#endif // UNIQUE_UVM_BYTEPRJLOADER_H_