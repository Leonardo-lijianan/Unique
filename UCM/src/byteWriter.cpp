// Copyright 2022 Source Speace Studio
// License(GPLv3.0)
// Author: Kkasi
// This is writing btyeCode into a file.

#include <fstream>
#include "byteWriter.h"

namespace compiler {

ByteWriter::ByteWriter() {}

ByteWriter::ByteWriter(std::string workDir, std::string fileName)
	: workDir(workDir), fileName(fileName) {}

ByteWriter::~ByteWriter() {}

void ByteWriter::setWorkDir(std::string workDir) {
	this->workDir=workDir;
}

void ByteWriter::setCode(std::vector<byte> code) {
	this->code=code;
}

void ByteWriter::setCsp(std::vector<std::string> csp) {
	this->constantStringPool = csp;
}

void ByteWriter::writeHead(std::ofstream& outf) {
	const char head[] = {0x3C, 0x2B, 0x1C, 0x2E, 0x3C, 0x0E}; // unique
	outf.write(head,6);
}

void ByteWriter::writeConstantStringPool(std::ofstream& outf) {
	char *buffer = new char[constantStringPool.size()];
	usint before = 0;
	if (before<=256) {
		buffer[before++] = (byte)constantStringPool.size();
		printf("ByteWriter::writeConstantStringPool::::: %d",constantStringPool.size());
		for (usint i = 0; i < constantStringPool.size(); i++) {
			for (usint j = 0; j < constantStringPool.at(i).size(); j++) {
				/* code */
				buffer[before++] = constantStringPool.at(i).at(j);
			}
		}
		outf.write(buffer,before);
	}
	delete[] buffer;
}

void ByteWriter::writeCode(std::ofstream& outf) {
	char *buffer = new char[code.size()];
	for (usint i = 0; i < code.size(); i++) { buffer[i] = code.at(i); }
	outf.write(buffer,code.size());
	delete[] buffer;
}

void ByteWriter::writing() {
	std::ofstream outf(workDir+"/"+fileName,std::ios::out | std::ios::binary);
	
	writeHead(outf);
	writeConstantStringPool(outf);
	writeCode(outf);

	outf.close();
}

} // namespace compiler