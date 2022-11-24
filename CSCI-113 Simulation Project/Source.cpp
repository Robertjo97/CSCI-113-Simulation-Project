#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct cache_block {
	int set = 0;
	int valid_bit = 0;
	int history = 0;
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

int compute_mem_addr(instruction instr) {
	int byte_addr = instr.rs + instr.offset;
	int word_addr = byte_addr / 4;
	cout << "Word address: " << word_addr << endl;
	return word_addr;
}

int binaryToDecimal(int n)
{
	int num = n;
	int dec_value = 0;
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

void load_operation(int memory_address, int rt, vector<reg_block> regs, cache cache, vector <memory_block> memory) {

	int set = memory_address % 8;
	int tag = memory_address / 8;

	if (cache.block_0.at(set).valid_bit == 1 || cache.block_1.at(set).valid_bit == 1) {
		if()
	}


	/*cout << "Set: " << set << endl;
	cout << "Tag: " << tag << endl;
	cout << "Rt value: " << rt << endl;
	cout << "Register:  " << "$S" << rt - 16 << endl;*/


}

void store_operation(int memory_address, int rt, vector<reg_block> regs, cache cache, vector <memory_block> memory) {
	int set = memory_address % 8;
	int tag = memory_address / 8;
	cout << "Set: " << set << endl;
	cout << "Tag: " << tag << endl;
	cout << "Rt value: " << rt << endl;
	cout << "Register:  " << "$S" << rt - 16 << endl;
}

//void read_hit(); 
//void read_miss();
//void write_hit();  //WB
//void write_miss(); //no-write-allocate

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

	cout << instruction_decoded.op << endl;
	cout << instruction_decoded.rs << endl;
	cout << instruction_decoded.rt << endl;
	cout << instruction_decoded.offset << endl;
	//cout << endl;

	return instruction_decoded;
}

void execute(instruction instr, vector<reg_block> regs, cache cache, vector <memory_block> memory) {

	if (instr.op == 35) {
		cout << "LW" << endl;
		load_operation(compute_mem_addr(instr), instr.rt, regs, cache, memory);

	}
	else if (instr.op == 43) {
		cout << "SW" << endl;
		store_operation(compute_mem_addr(instr), instr.rt, regs, cache, memory);
	}
	else {
		cout << "Error reading opcode" << endl;
	}
	cout << endl;
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
		execute(decode(line), reg_file, cache, memory);
	}

	return 0;
}