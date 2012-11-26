#include <iostream>
#include <stdlib.h>
#include <cassert>
#include <vector>
using namespace std;

void usage(){
	cerr << "Usage: <exec> <# patterns> <max # nodes>" << endl;
	exit(0);
}

int main(int argc, char** argv){
	if(argc != 3){
		usage();
	}
	int n = atoi(argv[1]);
	int maxNumNodes = atoi(argv[2]);
	assert(maxNumNodes > 1);
	int numNodes, randomIdx;
	srand(time(NULL));
	for(int j = 0; j < n; ++j){ // generate n test patterns
		while(true){
			numNodes = rand() % maxNumNodes + 1;
			if(numNodes % 2 == 0)
				break;
		}
		cout << numNodes << endl;
		// init
		vector<int> nodeList;
		nodeList.reserve(numNodes);
		for(int i = numNodes-1; i >= 0; --i)
			nodeList.push_back(i);
		// randomly pair
		for(int i = 0, end = numNodes/2; i < end; ++i){
			cout << nodeList.back() << " ";
			nodeList.pop_back();
			randomIdx = rand() % nodeList.size();
			cout << nodeList[randomIdx] << endl;
			nodeList.erase(nodeList.begin() + randomIdx);
		}
		cout << 0 << endl;
	}
	return 0;
}
