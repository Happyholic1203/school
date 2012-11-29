/****************************************************************************
FileName     [ maxPlanarSubset.cpp ]
Synopsis     [ Algorithm PA #2, an iterative DP bottom-up implementation ]
Author       [ Yu-Cheng (Henry) Huang ]
Copyright    [ Copyleft(c) 2012 NTUEE, Taiwan ]
****************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
using namespace std;

#define MAX_NUM_NODES 200001

// definitions
struct Arc{
	Arc(int s, int f){
		start = s;
		finish = f;
	}
	int start;
	int finish;
};

enum Status{
	WITHIN,
	OUTSIDE,
	HIT,
	END
};

typedef vector<Arc> ArcList;

// for STL sorting
bool arcCompare(Arc a1, Arc a2){
	return a1.start < a2.start;
}

// helper functions
int idx(const int& x, const int& y);
void pick(const int&, const int&);

// global variables
int n; // number of activities
int cnt[MAX_NUM_NODES];
ArcList answerList;

int main(){
	int s, f, tmp, tmp2;
	while(true){
		cin >> n; // even
		if(cin.eof())
			break;
		if(n % 2 != 0){
			cerr << "Error: Illegal number of nodes!!!" << endl;
			exit(0);
		
		}
	
		// parsing
		for(int i = 0, end = n/2; i < end; ++i){
			cin >> s >> f;
			cnt[s] = f;
			cnt[f] = s;
		}

		// DP elements for iterative implementation
		Status* statusTable = new Status[n * n]; // for answer retrieval
		memset(statusTable, END, n * n * sizeof(Status));
		int* maxNumChordsTable = new int[n * n]; // memoization
		memset(maxNumChordsTable, 0, n * n * sizeof(int));
		// solve: bottom-up DP
		int i, k;
		for(int l = 2; l < n; ++l){
			for(int j = n-1; j >= l; --j){
				i = j - l;
				k = cnt[j];
				if(k > j || k < i){
					//statusTable[idx(i, j)] = OUTSIDE;
					maxNumChordsTable[idx(i, j)] = maxNumChordsTable[idx(i, j-1)];
				}
				else if(k == i){
					//statusTable[idx(i, j)] = HIT;
					maxNumChordsTable[idx(i, j)] = maxNumChordsTable[idx(i+1, j-1)] + 1;
				}
				else{
					tmp = maxNumChordsTable[idx(i, k-1)] + maxNumChordsTable[idx(k+1, j)] + 1;
					tmp2 = maxNumChordsTable[idx(i, j-1)];
					if(tmp > tmp2){
						//statusTable[idx(i, j)] = WITHIN;
						maxNumChordsTable[idx(i, j)] = tmp;
					}
					else{
						//statusTable[idx(i, j)] = OUTSIDE;
						maxNumChordsTable[idx(i, j)] = tmp2;
					}
				}
			}
		}
		cout << maxNumChordsTable[idx(0, n-1)] << endl;
		// retrieve trace
		stack<pair<int, int> > callStack;
		callStack.push(make_pair(0, n-1));
		while(!callStack.empty()){
			int i = callStack.top().first;
			int j = callStack.top().second;
			callStack.pop();
			switch(statusTable[idx(i, j)]){
				case END: continue;
				case HIT: 
					pick(i, j); 
					callStack.push(make_pair(i+1, j-1));
					continue;
				case OUTSIDE:
					callStack.push(make_pair(i, j-1));
					continue;
				case WITHIN:
					pick(j, cnt[j]);
					callStack.push(make_pair(i, cnt[j]-1));
					callStack.push(make_pair(cnt[j]+1, j));
					continue;
			}
		}
		// print trace
		sort(answerList.begin(), answerList.end(), arcCompare);
		for(int i = 0, end = answerList.size(); i < end; ++i)
			cout << answerList[i].start << " " << answerList[i].finish << endl;
		cin >> s; // absorb the 0
		delete[] maxNumChordsTable;
		answerList.clear();
	}
	return 0;
}

inline int idx(const int& i, const int& j){
	return (i * n) + j;
}

inline void pick(const int& i, const int& j){
	if(i < j)
		answerList.push_back(Arc(i, j));
	else
		answerList.push_back(Arc(j, i));
}
