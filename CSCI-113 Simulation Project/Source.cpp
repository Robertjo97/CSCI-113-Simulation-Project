#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct cache_block {
	bool valid_bit;
	vector <int> tag;
	vector <int> data;
};

int main() {
	string instruction;
	ifstream file;

	file.open("02-Input-object-code");
	while (!file.eof()) {
		getline(file, instruction);
		cout << instruction << endl << endl;
	}
	file.close();

	vector <cache_block> cache(16);
	return 0;
}