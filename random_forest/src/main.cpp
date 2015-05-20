#include <iostream>
#include <fstream>
#include <string>
#include "DataEntry.h"
#include "TrainingDataEntry.h"
#include "DecisionTree.h"
using namespace std;

vector<TrainingDataEntry> DecisionNode::trainingData = DecisionNode::loadTrainingDataFromFile("data/train_300.csv");
vector<DataEntry> DecisionTree::testingData = DecisionTree::loadTestingDataFromFile("data/test_300.csv");

int main()
{
	DecisionTree tree = DecisionTree();
	tree.trainTree(tree.rootNode);
	cout << "Tree Depth: " << tree.getTreeDepth(tree.rootNode) << endl;

	tree.judgeAllTrainingData();
	tree.judgeAllTestingData("data/500_100.csv");

	return 0;
}