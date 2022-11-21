#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct cache_block {
	int set = 0;
	int valid_bit = 0;
	int tag = 0;
	int data = 0;
};

struct cache {
	vector <cache_block> block_0;
	vector <cache_block> block_1;
};

struct memory_block {
	int address = 0;
	int data = 0;
};

struct reg_block {
	int name = 0;
	int data = 0;
};

struct instruction {
	int op = 0;
	int rs = 0;
	int rt = 0;
	int offset = 0;
};

int binaryToDecimal(int n)
{
	int num = n;
	int dec_value = 0;

	// Initializing base value to 1, i.e 2^0
	int base = 1;

	int temp = num;
	while (temp) {
		int last_digit = temp % 10;
		temp = temp / 10;

		dec_value += last_digit * base;

		base = base * 2;
	}

	return dec_value;
}

instruction decode(string instr) {
	instruction instruction_decoded;
	string op = "";
	string rs = "";
	string rt = "";
	string offset = "";
	for (int i = 0; i < 6; i++) {
		op += instr.at(i);
	}
	for (int i = 6; i < 11; i++) {
		rs += instr.at(i);
	}
	for (int i = 11; i < 16; i++) {
		rt += instr.at(i);
	}
	for (int i = 16; i < 32; i++) {
		offset += instr.at(i);
	}
	instruction_decoded.op = binaryToDecimal(stoi(op));
	instruction_decoded.rs = binaryToDecimal(stoi(rs));
	instruction_decoded.rt = binaryToDecimal(stoi(rt));
	instruction_decoded.offset = binaryToDecimal(stoi(offset));

	return instruction_decoded;

}

int main() {
	cache cache;
	cache.block_0.reserve(8);
	cache.block_1.reserve(8);

	for (int i = 0; i < 8; i++) {
		cache_block block;
		block.set = i;
		cache.block_0.push_back(block);
		cache.block_1.push_back(block);
	}

	/*for (int i = 0; i < cache.block_0.size(); i++) {
		cout << cache.block_0.at(i).set << "    " << cache.block_0.at(i).valid_bit << "    " << cache.block_0.at(i).tag << "    " << cache.block_0.at(i).data << "          ";
		cout << cache.block_1.at(i).set << "    " << cache.block_1.at(i).valid_bit << "    " << cache.block_1.at(i).tag << "    " << cache.block_1.at(i).data << "          " << endl;
	}*/

	vector <memory_block> memory;
	memory.reserve(128);
	for (int i = 0; i < 128; i++) {
		memory_block memblock;
		memblock.address = i;
		memblock.data = i + 5;

		memory.push_back(memblock);
	}
	/*for (int i = 0; i < memory.size(); i++) {
		cout << memory.at(i).address << "    " << memory.at(i).data << endl;
	}*/

	vector <reg_block> reg_file;
	reg_file.reserve(8);
	for (int i = 0; i < 8; i++) {
		reg_block block;
		block.name = i;
		reg_file.push_back(block);
	}
	/*for (int i = 0; i < reg_file.size(); i++) {
		cout << "$S" << reg_file.at(i).name << "    " << reg_file.at(i).data << endl;
	}*/

	string line;
	ifstream file;
	
	file.open("02-Input-object-code");
	while (!file.eof()) {
		getline(file, line);
		decode(line);
	}

	return 0;
}