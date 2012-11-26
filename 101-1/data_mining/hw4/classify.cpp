/****************************************************************************
FileName     [ classify.cpp ]
Synopsis     [ data mining hw#4]
Author       [ Yu-Cheng (Henry) Huang ]
Copyright    [ Copyleft(c) 2012 NTUEE, Taiwan ]
****************************************************************************/

#include <iostream>
#include <vector>
#include <cassert>
#include <stdlib.h>
#include <limits>
using namespace std;

enum Attribute{
	DEPARTMENT = 0, 
	AGE, 
	SALARY,
	STATUS,
	NUM_ATTRIBUTES
};

enum Department{
	SALES = 0, 
	SYSTEMS, 
	MARKETING, 
	SECRETARY,
	NUM_DEPARTMENTS
};

enum Age{
	A_21_25 = 0,
	A_26_30,
	A_31_35,
	A_36_40,
	A_41_45,
	A_46_50,
	NUM_AGES
};

enum Salary{
	S_26_30 = 0,
	S_31_35,
	S_36_40,
	S_41_45,
	S_46_50,
	S_66_70,
	NUM_SALARIES
};

enum Status{
	SENIOR,
	JUNIOR
};

class Tuple{
public:
	Tuple(Department d, Status stat, Age a, Salary s, int c){
		_department = d;
		_status = stat;
		_age = a;
		_salary = s;
		_count = c;
	}

	int getAttr(const Attribute& attr) const{
		switch(attr){
			case DEPARTMENT: return _department;
			case AGE: return _age;
			case SALARY: return _salary;
			case STATUS: return _status;
			default: assert(false);
		}
	}

private:
	Department _department;
	Status     _status;
	Age        _age;
	Salary     _salary;
	int        _count;
};

typedef vector<Tuple> TupleList;

int getNumLabels(const Attribute& attr){
	switch(attr){
		case DEPARTMENT: return NUM_DEPARTMENTS;
		case AGE: return NUM_AGES;
		case SALARY: return NUM_SALARIES;
	}
}


// private class for DecisionTree
class DecisionNode{
	friend class DecisionTree;
private:
	DecisionNode(){}
	DecisionNode(int label){ // leaf node
		_label = label;
	}
	DecisionNode(DecisionNode* parent, Attribute attr){
		_parent = parent;
		_attr = attr;
		_branchList = new DecisionNode[getNumLabels(attr)];
		_status = -1;
	}

	bool isLeaf() const{
		return (_yes == NULL) && (_no == NULL);
	}

	// data members
	DecisionNode*  _parent;
	DecisionNode** _branchList;
	Attribute      _attr;
	int            _label; // label for the attribute
	Status         _status;
};

class DecisionTree{	
public:
	DecisionTree(TupleList& tupleList){
		// TODO: generate the tree
		_tupleList = tupleList;
		memset(_doneAttrList, NUM_ATTRIBUTES, 0);
		_root = NULL;
		genTree(NULL, _root, tupleList);
	}

	void printTree() const{
		cout << "Decision tree: \n" << endl;
	}
private:
	DecisionNode* _root;
	bool          _doneAttrList[NUM_ATTRIBUTES];
	//TupleList     _tupleList;

	void genTree(DecisionNode* parent, DecisionNode* child, const TupleList& tupleList){
		int splitAttr;
		int minGiniIdx = INT_MAX;
		int giniIdx;
		// go over all attributes and compare the gini indices
		for(int i = 0; i < NUM_ATTRIBUTES; ++i){
			if(_doneAttrList[i])
				continue;
			giniIdx = getGiniIdx(tupleList, i);
			if(giniIdx < minGiniIdx){
				minGiniIdx = giniIdx;
				splitAttr = i;
			}
		}
		if(minGiniIdx == 0){
			child = new DecisionNode(parent, splitAttr);
			for(int label = 0; label < getNumLabels(splitAttr); ++label){
				for(int j = 0; j < tupleList.size(); ++j)
					if(tupleList[j].getAttr(splitAttr) == label)
						_child->_branchList[label] = new DecisionNode(tupleList[j].getAttr(splitAttr));
			}
			return;
		}
		else{
		}
	}
};

int main(){
	TupleList tupleList;
	tupleList.push_back(Tuple(SALES, SENIOR, A_31_35, S_46_50, 30));
	tupleList.push_back(Tuple(SALES, JUNIOR, A_26_30, S_26_30, 40));
	tupleList.push_back(Tuple(SALES, JUNIOR, A_31_35, S_31_35, 40));
	tupleList.push_back(Tuple(SYSTEMS, JUNIOR, A_21_25, S_46_50, 20));
	tupleList.push_back(Tuple(SYSTEMS, SENIOR, A_31_35, S_66_70, 5));
	tupleList.push_back(Tuple(SYSTEMS, JUNIOR, A_26_30, S_46_50, 3));
	tupleList.push_back(Tuple(SYSTEMS, SENIOR, A_41_45, S_66_70, 3));
	tupleList.push_back(Tuple(MARKETING, SENIOR, A_36_40, S_46_50, 10));
	tupleList.push_back(Tuple(MARKETING, JUNIOR, A_31_35, S_41_45, 4));
	tupleList.push_back(Tuple(SECRETARY, SENIOR, A_46_50, S_36_40, 4));
	tupleList.push_back(Tuple(SECRETARY, JUNIOR, A_26_30, S_26_30, 6));
	DecisionTree decisionTree(tupleList);
	decisionTree.printTree();
/*
	Tuple t(SALES, SENIOR, A_31_35, S_46_50, 30);
	cout << t.getAttr(SALARY) << endl;
*/
	return 0;
}
