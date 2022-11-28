#include <iostream>
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
	cache_block block_0[8];
	cache_block block_1[8];
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
	return word_addr;
}
int conversion(int array[], int len) {
	int output = 0;
	int power = 1;

	for (int i = 0; i < len; i++)
	{
		output += array[(len - 1) - i] * power;
		power *= 2;
	}

	return output;
}

void load_operation(int memory_address, int &rt, int (&regs)[8], cache& cache, int (&memory)[128]) {

	int set = memory_address % 8;
	int tag = memory_address / 8;

	if (cache.block_0[set].valid_bit == 1) {							//Case: block_0 [valid passed] [tag passed]
		if (cache.block_0[set].tag == tag) {
			regs[rt - 16] = cache.block_0[set].data;
			cache.block_0[set].history = 1;
			cache.block_1[set].history = 0;
			cout << "\tRead hit";
		}
		else if (cache.block_1[set].valid_bit == 1) {
			if (cache.block_1[set].tag == tag) {						//Case: block_0 [valid passed] [tag failed] block_1 [valid passed] [tag passed]
				cout << "\tRead hit";
				regs[rt - 16] = cache.block_1[set].data;
				cache.block_1[set].history = 1;
				cache.block_0[set].history = 0;
				
			}
			else if (cache.block_1[set].tag != tag) {						//Case: block_0 [valid passed] [tag failed] block_1 [valid passed] [tag failed]
				cout << "\tRead miss LRU";
				if (cache.block_0[set].history == 0) {
					cache.block_0[set].tag = tag;
					cache.block_0[set].data = memory[memory_address];
					regs[rt - 16] = cache.block_0[set].data;
					cache.block_0[set].history = 1;
					cache.block_1[set].history = 0;
				}																	//LRU
				else if (cache.block_1[set].history == 0) {
					cache.block_1[set].tag = tag;
					cache.block_1[set].data = memory[memory_address];
					regs[rt - 16] = cache.block_1[set].data;
					cache.block_1[set].history = 1;
					cache.block_0[set].history = 0;
				}
				else {
					cout << "\tError reading history";
				}
			}
		}

			else if (cache.block_1[set].valid_bit == 0) {					//Case: block_0 [valid passed] [tag failed] block_1 [valid failed]
			cout << "\tRead miss";
			cache.block_1[set].valid_bit = 1;
			cache.block_1[set].tag = tag;
			cache.block_1[set].data = memory[memory_address];
			regs[rt-16] = cache.block_1[set].data;
			cache.block_1[set].history = 1;
			cache.block_0[set].history = 0;

		}
	}


	else if (cache.block_0[set].valid_bit == 0) {				//Case: block_0 [valid failed]
		cout << "\tRead miss";
		cache.block_0[set].valid_bit = 1;
		cache.block_0[set].tag = tag;
		cache.block_0[set].data = memory[memory_address];
		regs[rt - 16] = cache.block_0[set].data;
		cache.block_0[set].history = 1;
		cache.block_1[set].history = 0;
	}

	else {
		cout << "\tError reading cache";
	}


}

void store_operation(int memory_address, int& rt, int(&regs)[8], cache& cache, int(&memory)[128]) {
	int set = memory_address % 8;
	int tag = memory_address / 8;

	if (cache.block_0[set].valid_bit == 1) {					//block_0 [valid passed] [tag passed]
		if (cache.block_0[set].tag == tag) {
			cout << "\tWrite hit";
			cache.block_0[set].data = regs[rt - 16];
			cache.block_0[set].history = 1;
			cache.block_1[set].history = 0;
		}
		else if (cache.block_1[set].valid_bit == 1) {			//block_0 [valid passed] [tag failed] block_1 [valid passed] [tag passed]
			if (cache.block_1[set].tag == tag) {
				cout << "\tWrite hit";
				cache.block_1[set].data = regs[rt - 16];
				cache.block_1[set].history = 1;
				cache.block_0[set].history = 0;
			}
			else if (cache.block_1[set].tag != tag) {			//block_0 [valid passed] [tag failed] block_1 [valid passed] [tag failed]
				//LRU WB stuff
				cout << "\tWrite hit WB";
				if (cache.block_0[set].history == 0) {
					memory[memory_address] = cache.block_0[set].data;
					cache.block_0[set].data = regs[rt - 16];
					cache.block_0[set].history = 1;
					cache.block_1[set].history = 0;
				}
				else if (cache.block_1[set].history == 0) {
					memory[memory_address] = cache.block_1[set].data;
					cache.block_1[set].data = regs[rt - 16];
					cache.block_1[set].history = 1;
					cache.block_0[set].history = 0;
				}
				else {
					cout << "\tError reading history";
				}
			}
		}
		else if (cache.block_1[set].valid_bit == 0) {			//block_0 [valid passed] [tag failed] block_1 [valid failed]
			cout << "\tWrite miss";
			memory[memory_address] = regs[rt - 16];
		}	
	}
	else if (cache.block_0[set].valid_bit == 0) {				//block_0 [valid failed]
		cout << "\tWrite miss";
		memory[memory_address] = regs[rt - 16];
	}
	else {
		cout << "\tError writing";
	}
}


instruction decode(string instr) {
	instruction instruction_decoded;
	int op[6] = { };
	int rs[5] = { };
	int rt[5] = { };
	int offset[16] = { };
	
	for (int i = 0; i < 6; i++) {
		op[i] = instr[i] - '0';
	}
	for (int i = 6; i < 11; i++) {
		rs[i - 6] = instr[i] - '0';
	}
	for (int i = 11; i < 16; i++) {
		rt[i - 11] = instr[i] - '0';
	}
	for (int i = 16; i < 32; i++) {
		offset[i - 16] = instr[i] - '0';
	}
	instruction_decoded.op = conversion(op, 6);
	instruction_decoded.rs = conversion(rs, 5);
	instruction_decoded.rt = conversion(rt, 5);
	instruction_decoded.offset = conversion(offset, 16);

	return instruction_decoded;
}

void execute(instruction instr, int (&regs)[8], cache& cache, int (&memory)[128]) {
	if (instr.op == 35) {
		//cout << "LW";
		load_operation(compute_mem_addr(instr), instr.rt, regs, cache, memory);

	}
	else if (instr.op == 43) {
		//cout << "SW";
		store_operation(compute_mem_addr(instr), instr.rt, regs, cache, memory);
	}
	else {
		cout << "Error reading opcode" << endl;
	}
	cout << endl;
}




int main() {
	
	cache cache;
	for (int i = 0; i < 8; i++) {
		cache.block_0[i].set = i;
		cache.block_1[i].set = i;
	}

	int memory[128] = { };
	for (int i = 0; i < 128; i++) {
		memory[i] = i + 5;
	}

	int reg_file[8] = { };
	
	string line;
	ifstream file;

	file.open("02-Input-object-code");

	
	while (!file.eof()) {
		getline(file, line);
		cout << line << endl;
		execute(decode(line), reg_file, cache, memory);
		cout << "--------------------------------" << endl;
	}
	cout << endl << "Register Contents:" << endl;
	for (int i = 0; i < 8; i++) {
		cout << "$S" << i << "\t" << reg_file[i] << endl;
	}
	cout << endl << "Cache Block 0\t\t\t\t\t\tCache Block 1 " << endl;
	cout << "Set#\tValid\tTag\tData\t\t\t\tSet#\tValid\tTag\tData" << endl;
	for (int i = 0; i < 8; i++) {
		cout << cache.block_0[i].set << "\t" << cache.block_0[i].valid_bit << "\t" << cache.block_0[i].tag << "\t" << cache.block_0[i].data;
		cout <<"\t\t\t\t" << cache.block_1[i].set << "\t" << cache.block_1[i].valid_bit << "\t" << cache.block_1[i].tag << "\t" << cache.block_1[i].data << endl;
	}
	
	return 0;
}