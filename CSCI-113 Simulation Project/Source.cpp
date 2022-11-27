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
	//cout << "Word address: " << word_addr << endl;
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

	//cout << instruction_decoded.op << endl;
	//cout << instruction_decoded.rs << endl;
	//cout << instruction_decoded.rt << endl;
	//cout << instruction_decoded.offset << endl;
	////cout << endl;

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
		cout << line;
		execute(decode(line), reg_file, cache, memory);
	}

	/*for (int i = 0; i < 8; i++) {
		cout << "Register Contents:" << endl;
		cout << reg_file[i] << endl;
	}*/
	int n;
	cin >> n;
	
	return 0;
}