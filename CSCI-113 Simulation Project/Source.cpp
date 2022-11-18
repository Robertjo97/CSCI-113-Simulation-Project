#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct cache_block {
	bool valid_bit = false;
	vector <int> tag;
	vector <int> data;
};

struct memory_block {
	vector<int> block;
};

int main() {
	vector <cache_block> cache;
	cache.reserve(16);
	string line;
	ifstream file;
	vector<int> instruction;

	file.open("02-Input-object-code");
	while (!file.eof()) {
		getline(file, line);
		for (int i = 0; i < 32; i++) {
			instruction.push_back(line[i] - '0');
		}
	}
	vector <memory_block> main_memory;
	main_memory.reserve(128);
	for (int i = 0; i < main_memory.size(); i++) {
		main_memory.at(i).block.reserve(32);
	}
	

	/*cout << line[0];
	int n = line[0] - '0';
	cout << n;
	while (!file.eof()) {
		
		
	}
	file.close();*/

	
	return 0;
}