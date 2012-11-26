#include <iostream>
#include <fstream>
using namespace std;

void usage(){
	cerr << "Usage: <exec> <input> <output>" << endl;
	exit(0);
}

void fail2Open(char* fileName){
	cerr << "Failed to open file: " << fileName << endl;
}

int main(int argc, char** argv){
	if(argc != 3){
		usage();
	}

	ifstream input(argv[1]), output(argv[2]);
	if(!input.is_open())
		fail2Open(argv[1]);
	if(!output.is_open())
		fail2Open(argv[2]);

	int n, s, f;
	while(true){
		input >> n;
		if(input.eof())
			break;
		cout << "input:" << endl;
		cout << n << endl;
		for(int i = 0, end = n/2; i < end; ++i){
			input >> s >> f;
			cout << s << " " << f << endl;
		}
		input >> s; // absorb the 0

		cout << "output:" << endl;
		output >> n;
		cout << n << endl;
		for(int i = 0; i < n; ++i){
			output >> s >> f;
			cout << s << " " << f << endl;
		}
		cout << "----" << endl;
	}
	return 0;
}
