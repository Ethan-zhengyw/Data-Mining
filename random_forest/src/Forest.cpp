#include <thread>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "Forest.h"

Forest::Forest(int n) {
	treeNum = n;
	for (int i = 0; i < treeNum; i++) {
		trees.push_back(DecisionTree());
		cout << i << " tree rootNode size: " << trees[i].rootNode->entryIds.size() << endl;
	}
}

void Forest::trainAllTrees(int threadNum) {
	vector<thread> trainingPool;

	for (int i = 0; i < treeNum; i += threadNum) {
		int start = i;
		for (int j = 0; j < threadNum && i + j < treeNum; j++) {
			trainingPool.push_back(thread(trainTree, &trees[i + j]));
		}
		for (int j = 0; j < threadNum && i + j < treeNum; j++) {
			trainingPool[i + j].join();
		}
	}
}

void Forest::trainTree(DecisionTree* tree) {
	tree->trainTree(tree->rootNode);
}

int Forest::judgeOne(DataEntry* entry, int useTreeNum) {
	int label = 0, labels[26] = { 0 };

	for (int i = 0; i < useTreeNum; i++) {
		label = trees[i].judgeOne(entry);
		labels[label - 1]++;
	}

	for (int i = 0; i < 26; i++)
		label = (labels[i] > labels[label]) ? i : label;
	label += 1;

	return label;
}

float Forest::judgeAllTrainingData(int useTreeNum) {
	int label, correctNum = 0, size = DecisionNode::trainingData.size();
	float correctRate;

	for (int i = 0; i < size; i++) {
		label = judgeOne(&DecisionNode::trainingData[i], useTreeNum);
		if (label == DecisionNode::trainingData[i].getLabel())
			correctNum++;
	}

	correctRate = (float)correctNum / size;
	cout << "Use " << useTreeNum << ": " << correctNum << " / " << size << " = " << correctRate << endl;

	return correctRate;
}

void Forest::judgeAllTestingData(string path, int useTreeNum) {
	int label, size = DecisionTree::testingData.size();
	ofstream ofs = ofstream(path);

	ofs << "id,label" << endl;
	for (int i = 0; i < size; i++) {
		label = judgeOne(&DecisionTree::testingData[i], useTreeNum);
		ofs << DecisionTree::testingData[i].getId() << "," << label << endl;
	}

	ofs.close();
}