#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define MAX_NUM_NODES 100001

// definitions
struct Arc{
	Arc(int s, int f){
		start = s;
		finish = f;
	}
	int start;
	int finish;
};

typedef vector<Arc> ArcList;

// utility functions
bool arcCompare(Arc a1, Arc a2){
	return a1.start < a2.start;
}
int rotate(int x);
void solve(int i, int j);

// global variables
ArcList arcList, answerList;
int n; // number of nodes
int cnt[MAX_NUM_NODES];
int offset;

int main(){
	int s, f, maxNumArc, prev, save, maxOffset;
	ArcList maxList;
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
			//arcList.push_back(Arc(s, f));
			cnt[s] = f;
			cnt[f] = s;
		}
		// solve
		maxNumArc = -1;
		maxOffset = -1;

		// for debugging
		/*
		cout << "originally: " << endl;
		for(int i = 0; i < n; ++i)
			cout << i << " " << cnt[i] << endl;
		*/

		for(int i = 0; i < n; ++i){
			offset = i;
			solve(0, n-1);
			if(int(answerList.size()) > maxNumArc){
				maxNumArc = answerList.size();
				maxList = answerList;
				maxOffset = i;
			}
			answerList.clear();
			// rotate the circle
			prev = cnt[0];
			cnt[0] = (cnt[n-1] + 1) % n;
			for(int j = 1; j < n; ++j){
				save = cnt[j];
				cnt[j] = (prev + 1) % n;
				prev = save;
			}
			// for debugging
			/*
			cout << "maxOffset = " << maxOffset << endl;
			for(int i = 0; i < n; ++i)
				cout << i << " " << cnt[i] << endl;
			*/
		}
		// print answers
		sort(maxList.begin(), maxList.end(), arcCompare);
		cout << maxList.size() << endl;
		for(int i = 0, end = maxList.size(); i < end; ++i){
			//cout << (maxList[i].start + (n - maxOffset)) % n << " " << (maxList[i].finish + (n - maxOffset)) % n << endl;
			cout << maxList[i].start << " " << maxList[i].finish << endl;
		}
		cin >> s; // absorb the 0
		arcList.clear();
		answerList.clear();
	}
	return 0;
}

// DP
void solve(int i, int j){
	if(i >= j) // tricky case
		return;
	int k = cnt[j];
	if(k == i){
		answerList.push_back(Arc(rotate(i), rotate(j)));
		solve(i+1, j-1);
	}
	else if(k > j || k < i){ // k < i means it's outside the shadow
		solve(i, j-1);
	}
	else{ // k < j
		answerList.push_back(Arc(rotate(k), rotate(j)));
		solve(i, k-1);
		solve(k+1, j);
	}
}

int rotate(int x){
	return (x + (n - offset)) % n;
}
