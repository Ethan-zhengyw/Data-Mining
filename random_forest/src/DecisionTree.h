#ifndef _DECISIONTREE_H_
#define _DECISIONTREE_H_

#include <vector>
#include "TrainingDataEntry.h"
using namespace std;

struct DecisionNode
{
	int attrId;
	float attrVal;
	vector<int> entryIds;

	DecisionNode* left;
	DecisionNode* right;
};

class DecisionTree
{
public:
	DecisionTree();
	static vector<TrainingDataEntry> loadTrainingDataFromFile(string path);

private:
	DecisionNode* rootNode;
	static vector<TrainingDataEntry> trainingData;
};

#endif