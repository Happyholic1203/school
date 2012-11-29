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
#include <map>
using namespace std;

#define MAX_NUM_NODES 200001
#define THRESHOLD 22000

// definitions
struct Arc{
	Arc(int s, int f){
		if(s < f){
			start = s;
			finish = f;
		}
		else{
			start = f;
			finish = s;
		}
	}
	int start;
	int finish;
};

enum Status{
	END,
	WITHIN,
	OUTSIDE,
	HIT
};

typedef vector<Arc> ArcList;

// for STL sorting
bool arcCompare(Arc a1, Arc a2){
	return a1.start < a2.start;
}

// helper functions
int solve(const int&, const int&);
void retrieve(const int&, const int&);
int& table(const int&, const int&);
void pick(const int&, const int&);
void unPick(const int&, const int&);
bool isPicked(const int&, const int&);

// global variables
int n; // number of nodes
int cnt[MAX_NUM_NODES];
int maxNumChordsTable[THRESHOLD][THRESHOLD]; // memoization
vector<int> largeTable[MAX_NUM_NODES]; // for large inputs
bool smallPick[THRESHOLD][THRESHOLD];
vector<bool> largePick[MAX_NUM_NODES];
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

		// init
		for(int i = 0; i < n; ++i){
			for(int j = i+1; j < n; ++j){
				table(i, j) = -1;
				unPick(i, j);
			}
		}

		tmp = solve(0, n-1);
		cout << tmp << endl;
		answerList.reserve(tmp);
		retrieve(0, n-1);

		// print trace
		sort(answerList.begin(), answerList.end(), arcCompare);
		for(int i = 0, end = answerList.size(); i < end; ++i)
			cout << answerList[i].start << " " << answerList[i].finish << endl;
		cin >> s; // absorb the 0
		answerList.clear();
	}
	return 0;
}

inline int& table(const int& i, const int& j){
	if(i < THRESHOLD && j < THRESHOLD)
		return maxNumChordsTable[i][j];
	else{
		if(largeTable[i].size() == 0)
			largeTable[i].resize(n - i);
		return largeTable[i][j - i];
	}
}

inline void pick(const int& i, const int& j){
	if(i < THRESHOLD && j < THRESHOLD)
		smallPick[i][j] = true;
	else{
		if(largePick[i].size() == 0)
			largePick[i].resize(n - i);
		largePick[i][j - i] = true;
	}
}

inline void unPick(const int& i, const int& j){
	if(i < THRESHOLD && j < THRESHOLD)
		smallPick[i][j] = false;
	else{
		if(largePick[i].size() == 0)
			largePick[i].resize(n - i);
		largePick[i][j - i] = false;
	}
}

inline bool isPicked(const int& i, const int& j){
	if(i < THRESHOLD && j < THRESHOLD)
		return smallPick[i][j];
	else{
		return largePick[i][j - i];
	}
}

int solve(const int& i, const int& j){
	if(i >= j || i < 0 || j < 0)
		return 0;
	if(table(i, j) != -1)
		return table(i, j);
	if(cnt[j] < j && cnt[j] >= i){
		int tmp1 = 1 + solve(i, cnt[j]) + solve(cnt[j]+1, j-1);
		int tmp2 = solve(i, j-1);
		if(tmp2 > tmp1){
			return table(i, j) = tmp2;
		}
		else{
			//pick[i][j] = true;
			pick(i, j);
			return table(i, j) = tmp1;
		}
	}
	else{
		return table(i, j) = solve(i, j-1);
	}
}

void retrieve(const int& i, const int& j){
	if(i >= j || i < 0 || j < 0)
		return;
	if(!isPicked(i, j))
		retrieve(i, j-1);
	else{
		answerList.push_back(Arc(j, cnt[j]));
		retrieve(i, cnt[j]);
		retrieve(cnt[j]+1, j-1);
	}
}
