#include <iostream>
#include <fstream>
#include <string>
#include "DataEntry.h"
#include "TrainingDataEntry.h"
#include "DecisionTree.h"
#include "Forest.h"
using namespace std;

vector<TrainingDataEntry> DecisionNode::trainingData = DecisionNode::loadTrainingDataFromFile("data/train.csv");
vector<DataEntry> DecisionTree::testingData = DecisionTree::loadTestingDataFromFile("data/test.csv");
int DecisionTree::alpha = 0.001 * DecisionNode::trainingData.size();  // 属性分割100段
int DecisionTree::beta = 0.001 * DecisionNode::trainingData.size();  // 节点大小1%，60左右

/*	alpha	beta	tree	cr
 *	0.05	0.05	1		45.56
 *	0.05	0.05	5		48.33
 *	0.05	0.05	10		48.70
 *	0.01	0.05	1		72.89
 *	0.01	0.05	5		74.09
 *	0.01	0.05	10		75.23
 *	0.01	0.01	1		75.77
 *	0.01	0.01	5		79.90
 *	0.01	0.01	9		80.12

 *	0.01	0.01	1		75.80
 *	0.01	0.01	5		79.27
 *	0.01	0.01	10		79.58
 *	0.01	0.01	15		80.19
 *	0.01	0.01	20		81.30

 *	0.01	0.001	1		75.90
 *	0.01	0.001	5		81.12
 *	0.01	0.001	9		81.13

 *	0.001	0.001	1		88.84
 *	0.001	0.001	6		93.51
 *	0.001	0.001	11		94.62
 *	0.001	0.001	16		95.05
 */

int main()
{
	int forestSize = 140, threadNum = 7;
	Forest forest = Forest(forestSize);
	forest.trainAllTrees(threadNum);

	
	for (int i = 0; i < forestSize; i++) {
		forest.trees[i].judgeAllTrainingData();
	}


	for (int i = 1; i <= forestSize; i++) {
		forest.judgeAllTrainingData(i);
		forest.judgeAllTestingData("data/tmp.csv", i);

		ofstream tmp("data/treeNum.txt");
		tmp << i;
		tmp.close();
		ifstream tmp2("data/treeNum.txt");
		string tm;
		tmp2 >> tm;
		tmp2.close();
		string path = "data/data/gini0.001_0.001_0.001_9_6_" + tm + ".csv";
		forest.judgeAllTestingData(path, i);
	}
	return 0;
}