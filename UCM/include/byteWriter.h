// Copyright 2022 Source Speace Studio
// License(GPLv3.0)
// Author: Kkasi
// This is writing btyeCode into a file.

#ifndef UNIQUE_UCM_BYTEWRITER_H_
#define UNIQUE_UCM_BYTEWRITER_H_

#include <string>
#include <vector>
#include "def.h"

namespace compiler {

class ByteWriter
{
private:
	std::string workDir;
	std::string fileName;
	std::vector<byte> code;
	std::vector<std::string> constantStringPool;
public:
	ByteWriter();
	ByteWriter(std::string workDir, std::string fileName);
	~ByteWriter();

	void setWorkDir(std::string workDir);
	void setCode(std::vector<byte> code);
	void setCsp(std::vector<std::string> csp);
	void writeHead(std::ofstream& outf);
	void writeConstantStringPool(std::ofstream& outf);
	void writeCode(std::ofstream& outf);
	void writing();
};

} // namespace compiler

#endif // UNIQUE_UCM_BYTEWRITER_H_