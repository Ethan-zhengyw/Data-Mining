﻿#include <iostream>
#include <fstream>
#include <set>
#include "DecisionTree.h"

struct attr_cmp {
	bool operator()(float x, float y) { return x > y; }
};
vector<TrainingDataEntry> DecisionNode::trainingData = DecisionNode::loadTrainingDataFromFile("data/train_300.csv");

vector<TrainingDataEntry> DecisionNode::loadTrainingDataFromFile(string path) {
	vector<TrainingDataEntry> tde;
	string csvLine;
	ifstream fin = ifstream(path);

	getline(fin, csvLine);  // 第一行是title

	while (getline(fin, csvLine))
		tde.push_back(TrainingDataEntry(csvLine));

	return tde;
}

void DecisionNode::calGiniIndex() {
	int labels[DataEntry::labelNum] = {0};  
	int entrysNum = entryIds.size();

	for (int i = 0; i < entrysNum; i++) {
		labels[trainingData[entryIds[i]].getLabel() - 1]++;  // label 从 1 到 26
	}

	giniIndex = 0;
	for (int i = 0; i < DataEntry::labelNum; i++) {
		if (labels[i] != 0)
			giniIndex += (labels[i] * labels[i]);
	}
	giniIndex = 1 - (giniIndex / (entrysNum * entrysNum));
}

int DecisionTree::getTreeDepth(DecisionNode* node) {
	if (!node)
		return 0;

	int depth_left = getTreeDepth(node->left);
	int depth_right = getTreeDepth(node->right);

	return (depth_left > depth_right) ? (depth_left + 1) : (depth_right + 1);
}

DecisionTree::DecisionTree() {
	rootNode = new DecisionNode();
	for (unsigned i = 0; i < DecisionNode::trainingData.size(); i++) {
		rootNode->entryIds.push_back(i);
	}
	rootNode->calGiniIndex();
}

void DecisionTree::trainTree(DecisionNode* node) {
	if (!node
		// || 信息增益 < 阈值
		|| node->giniIndex < 0.1  // 满意的基尼系数
		|| getTreeDepth(rootNode) >= 300
		|| node->entryIds.size() < 10)  {// 最大决策树深度
		node->attrId = -1;
		return;
	}

	// 1. 寻找当前节点最优划分属性id和属性值
	set<float> attrs_;
	set<float>::iterator iter;
	float targetVal, averGini = node->giniIndex;
	int entryNum, targetId, alpha;  // alpha 代表从候选分割值集合下标处的值每次递增的值
	DecisionNode *left = NULL, *right = NULL;

	entryNum = node->entryIds.size();
	node->attrId = -1;
	for (int i = 0; i < DataEntry::attrNum; i++) {
		attrs_.clear();
		alpha = 5;  // 设置间隔
		for (int j = 0; j < entryNum; j++) {
			attrs_.insert(DecisionNode::trainingData[node->entryIds[j]].getValAt(i));
		}
		// set<float, attr_cmp> attrs(attrs_.begin(), attrs_.end());  // 默认就有序了，所以不必再排序
		iter = attrs_.begin();
		for (targetId = alpha; targetId + alpha < attrs_.size(); targetId += alpha) {
			for (int t = 0; t < alpha; t++)
				iter++;
			targetVal = *iter;
			left = new DecisionNode();
			right = new DecisionNode();
			for (int k = 0; k < entryNum; k++) {
				if (DecisionNode::trainingData[node->entryIds[k]].getValAt(i) < targetVal) {
					left->entryIds.push_back(node->entryIds[k]);
				} else {
					right->entryIds.push_back(node->entryIds[k]);
				}
			}
			left->calGiniIndex();
			right->calGiniIndex();
			float newAverGini = (left->giniIndex * left->entryIds.size()
				+ right->giniIndex * right->entryIds.size()) / entryNum;
			if (newAverGini < averGini) {
				node->attrId = i;
				node->attrVal = targetVal;
				averGini = newAverGini;
			}
		}
	}

	if (node->attrId == -1 || left->entryIds.size() < 5 || right->entryIds.size() < 5)
		return;

	cout << node->giniIndex << " " << averGini << " " << node->entryIds.size() << endl;
	// 2. 根据1.中的划分方法创建左右节点
	node->left = new DecisionNode();
	node->right = new DecisionNode();
	for (int i = 0; i < entryNum; i++) {
		if (DecisionNode::trainingData[node->entryIds[i]].getValAt(node->attrId) < node->attrVal) {
			node->left->entryIds.push_back(node->entryIds[i]);
		}
		else {
			node->right->entryIds.push_back(node->entryIds[i]);
		}
	}
	node->left->calGiniIndex();
	node->right->calGiniIndex();

	// 3. trainTree left and right
	trainTree(node->left); 
	trainTree(node->right);
}