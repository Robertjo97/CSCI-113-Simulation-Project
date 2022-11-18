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

int main() {
	vector <cache_block> cache(16);
	string line;
	ifstream file;
	vector<int> test;

	file.open("02-Input-object-code");
	getline(file, line);
	for (int i = 0; i < 32; i++) {
		test.push_back(line[i] - '0');
	}
	for (int i = 0; i < test.size(); i++) {
		cout << test.at(i);
	}
	/*cout << line[0];
	int n = line[0] - '0';
	cout << n;
	while (!file.eof()) {
		
		
	}
	file.close();*/

	
	return 0;
}