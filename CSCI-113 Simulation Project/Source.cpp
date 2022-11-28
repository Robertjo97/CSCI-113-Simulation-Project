#include <iostream>
#include <fstream>
#include <string>

/*
Robert Orlando
11/27/2022
CSCI-113
Memory Simulation Project
*/

using namespace std;

struct cache_block {				//Creates cache block structure
	int set = 0;
	int valid_bit = 0;
	int history = 0;
	int tag = 0;
	int data = 0;
};

struct cache {						//Creates 2 way set-associative cache
	cache_block block_0[8];
	cache_block block_1[8];
};

struct instruction {				//Holds instruction information
	int op = 0;
	int rs = 0;
	int rt = 0;
	int offset = 0;
};

int compute_mem_addr(instruction instr) {			//Computes memory address from instruction information
	int byte_addr = instr.rs + instr.offset;
	int word_addr = byte_addr / 4;
	return word_addr;
}
int conversion(int array[], int len) {				//Converts binary arrays into decimal integers
	int output = 0;
	int power = 1;

	for (int i = 0; i < len; i++)
	{
		output += array[(len - 1) - i] * power;
		power *= 2;
	}

	return output;
}

void FourBitBin(int n)								//Converts decimal integers into 4 bit binary numbers
{
	int binaryNum[32] = { };

	int i = 0;
	while (n > 0) {

		binaryNum[i] = n % 2;
		n = n / 2;
		i++;
	}

	for (int j = 3; j >= 0; j--)
		cout << binaryNum[j];
}

void ThirtyTwoBitBin(int n)							//Converts decimal integers into 32 bit binary numbers
{
	int binaryNum[32] = { };

	int i = 0;
	while (n > 0) {

		binaryNum[i] = n % 2;
		n = n / 2;
		i++;
	}

	for (int j = 31; j >= 0; j--)
		cout << binaryNum[j];
}

void SevenBitBin(int n)								//Converts decimal integers into 7 bit binary numbers
{
	int binaryNum[32] = { };

	int i = 0;
	while (n > 0) {

		binaryNum[i] = n % 2;
		n = n / 2;
		i++;
	}

	for (int j = 6; j >= 0; j--)
		cout << binaryNum[j];
}

void load_operation(int memory_address, int &rt, int (&regs)[8], cache& cache, int (&memory)[128]) {	//Load operation function. Contains all logic for LW instruction.

	int set = memory_address % 8;
	int tag = memory_address / 8;

	if (cache.block_0[set].valid_bit == 1) {							//Case: block_0 [valid passed] [tag passed]
		if (cache.block_0[set].tag == tag) {
			regs[rt - 16] = cache.block_0[set].data;
			cache.block_0[set].history = 1;
			cache.block_1[set].history = 0;
			cout << "\thit";
		}
		else if (cache.block_1[set].valid_bit == 1) {
			if (cache.block_1[set].tag == tag) {						//Case: block_0 [valid passed] [tag failed] block_1 [valid passed] [tag passed]
				cout << "\thit";
				regs[rt - 16] = cache.block_1[set].data;
				cache.block_1[set].history = 1;
				cache.block_0[set].history = 0;
				
			}
			else if (cache.block_1[set].tag != tag) {						//Case: block_0 [valid passed] [tag failed] block_1 [valid passed] [tag failed]
				cout << "\tmiss";
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
			cout << "\tmiss";
			cache.block_1[set].valid_bit = 1;
			cache.block_1[set].tag = tag;
			cache.block_1[set].data = memory[memory_address];
			regs[rt-16] = cache.block_1[set].data;
			cache.block_1[set].history = 1;
			cache.block_0[set].history = 0;

		}
	}


	else if (cache.block_0[set].valid_bit == 0) {				//Case: block_0 [valid failed]
		cout << "\tmiss";
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

void store_operation(int memory_address, int& rt, int(&regs)[8], cache& cache, int(&memory)[128]) {	//Store operation function. Contains all logic for SW instruction.
	int set = memory_address % 8;
	int tag = memory_address / 8;

	if (cache.block_0[set].valid_bit == 1) {					//block_0 [valid passed] [tag passed]
		if (cache.block_0[set].tag == tag) {
			cout << "\thit";
			cache.block_0[set].data = regs[rt - 16];
			cache.block_0[set].history = 1;
			cache.block_1[set].history = 0;
		}
		else if (cache.block_1[set].valid_bit == 1) {			//block_0 [valid passed] [tag failed] block_1 [valid passed] [tag passed]
			if (cache.block_1[set].tag == tag) {
				cout << "\thit";
				cache.block_1[set].data = regs[rt - 16];
				cache.block_1[set].history = 1;
				cache.block_0[set].history = 0;
			}
			else if (cache.block_1[set].tag != tag) {			//block_0 [valid passed] [tag failed] block_1 [valid passed] [tag failed]
				cout << "\thit";
				if (cache.block_0[set].history == 0) {
					memory[memory_address] = cache.block_0[set].data;
					cache.block_0[set].data = regs[rt - 16];
					cache.block_0[set].history = 1;
					cache.block_1[set].history = 0;
				}													//LRU WB
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
			cout << "\tmiss";
			memory[memory_address] = regs[rt - 16];
		}	
	}
	else if (cache.block_0[set].valid_bit == 0) {				//block_0 [valid failed]
		cout << "\tmiss";
		memory[memory_address] = regs[rt - 16];
	}
	else {
		cout << "\tError writing";
	}
}


instruction decode(string instr) {				//Instruction decode function. 
	instruction instruction_decoded;			//Takes a line from the machine instruction file and stores sections into instruction struct. 
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

void execute(instruction instr, int (&regs)[8], cache& cache, int (&memory)[128]) {	//Executes proper function based on instruction opcode.
	if (instr.op == 35) {
		load_operation(compute_mem_addr(instr), instr.rt, regs, cache, memory);

	}
	else if (instr.op == 43) {
		store_operation(compute_mem_addr(instr), instr.rt, regs, cache, memory);
	}
	else {
		cout << "Error reading opcode" << endl;
	}
	cout << endl;
}


int main() {
	
	cache cache;							//Creates and initializes cache
	for (int i = 0; i < 8; i++) {
		cache.block_0[i].set = i;
		cache.block_1[i].set = i;
	}

	int memory[128] = { };					//Creates and initializes memory
	for (int i = 0; i < 128; i++) {
		memory[i] = i + 5;
	}

	int reg_file[8] = { };					//Creates and initializes registers 
	
	string line;
	ifstream file;

	file.open("02-Input-object-code");

	
	while (!file.eof()) {					//Reading and executing file instructions
		getline(file, line);
		cout << line << endl;
		execute(decode(line), reg_file, cache, memory);
		cout << "--------------------------------" << endl;
	}
	
	cout << endl << "Registers" << endl;		//Displays registers
	for (int i = 0; i < 8; i++) {
		cout << "$S" << i << "\t"; 
		ThirtyTwoBitBin(reg_file[i]);
		cout << endl;
	}
	
	cout << endl << "Cache Block 0\t\t\t\t\t\t\tCache Block 1 " << endl;			//Displays cache
	cout << "Set#\tValid\tTag\tData\t\t\t\t\tSet#\tValid\tTag\tData" << endl;
	for (int i = 0; i < 8; i++) {
		cout << cache.block_0[i].set << "\t" << cache.block_0[i].valid_bit << "\t"; FourBitBin(cache.block_0[i].tag); cout << "\t"; ThirtyTwoBitBin(cache.block_0[i].data);
		cout << "\t" << cache.block_1[i].set << "\t" << cache.block_1[i].valid_bit << "\t"; FourBitBin(cache.block_1[i].tag); cout << "\t"; ThirtyTwoBitBin(cache.block_1[i].data);
		cout << endl;
	}
	
	cout << endl << "Memory" << endl;		//Displays memory
	cout << "Address\t\tData" << endl;
	for (int i = 0; i < 128; i++) {
		SevenBitBin(i);
		cout << "\t\t";
		ThirtyTwoBitBin(memory[i]);
		cout << endl;
	}
	return 0;
}