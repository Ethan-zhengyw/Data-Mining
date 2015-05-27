#include <iostream>
#include <fstream>
#include <set>
#include <ctime>
#include "DecisionTree.h"

using namespace std;

int DecisionTree::seed = 0;

struct attr_cmp {
	bool operator()(float x, float y) { return x > y; }
};

vector<TrainingDataEntry> DecisionNode::loadTrainingDataFromFile(string path) {
	cout << "loading traing data from file " << path << "..." << endl;
	vector<TrainingDataEntry> tde;
	string csvLine;
	ifstream fin = ifstream(path);

	getline(fin, csvLine);  // 第一行是title

	while (getline(fin, csvLine))
		tde.push_back(TrainingDataEntry(csvLine));

	cout << "done." << endl;
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
	int size = DecisionNode::trainingData.size();
	rootNode = new DecisionNode();
	srand(time(NULL) + (seed++));

	for (unsigned i = 0; i < size; i++) {
		if ((rand() % 10) < 9)  // 随机抽取90%的数据用来训练
			rootNode->entryIds.push_back(DecisionNode::trainingData[i].getId());
	}

	rootNode->calGiniIndex();
	alpha = (alpha > 5) ? alpha : 5;
	beta = (beta > 10) ? beta : 10;
}

void DecisionTree::trainTree(DecisionNode* node) {
	if (!node
		// || 信息增益 < 阈值
		|| node->giniIndex < 0.01  // 满意的基尼系数
		|| getTreeDepth(rootNode) >= 30  // 最大决策树深度
		|| node->entryIds.size() < beta)  {
		node->attrId = -1;
		return;
	}

	// 1. 寻找当前节点最优划分属性id和属性值
	set<float> attrs_;
	set<float>::iterator iter;
	float targetVal, averGini = node->giniIndex;
	int entryNum, targetId;  
	DecisionNode left, right;


	entryNum = node->entryIds.size();
	node->attrId = -1;
	srand(time(NULL) + (seed++));
	for (int i = 0; i < DataEntry::attrNum; i++) {
		if (usedAttrs.find(i) != usedAttrs.end()  // 属性还没用过
			&& (rand() % 10) >= 6)  // 只有60%的属性会被选来
			continue;
		attrs_.clear();
		for (int j = 0; j < entryNum; j++) {
			attrs_.insert(DecisionNode::trainingData[node->entryIds[j]].getValAt(i));
		}
		// set<float, attr_cmp> attrs(attrs_.begin(), attrs_.end());  // 默认就有序了，所以不必再排序
		iter = attrs_.begin();
		for (targetId = alpha; targetId + alpha < attrs_.size(); targetId += alpha) {
			for (int t = 0; t < alpha; t++)
				iter++;
			targetVal = *iter;
			left = DecisionNode();
			right = DecisionNode();
			for (int k = 0; k < entryNum; k++) {
				if (DecisionNode::trainingData[node->entryIds[k]].getValAt(i) < targetVal) {
					left.entryIds.push_back(node->entryIds[k]);
				} else {
					right.entryIds.push_back(node->entryIds[k]);
				}
			}
			left.calGiniIndex();
			right.calGiniIndex();
			float newAverGini = (left.giniIndex * left.entryIds.size()
				+ right.giniIndex * right.entryIds.size()) / entryNum;
			if (newAverGini < averGini
				&& left.entryIds.size() > beta / 3
				&& right.entryIds.size() > beta / 3
				&& node->giniIndex - newAverGini > 0.0005
				) {
				node->attrId = i;
				node->attrVal = targetVal;
				averGini = newAverGini;
			}
			// delete left; delete right;
		}
	}

	if (node->attrId == -1)
		return;

	usedAttrs.insert(node->attrId);

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

	std::cout << node->giniIndex << "-->" << averGini << ":"
		<< node->entryIds.size() << "\t" << node->left->entryIds.size() << ":" << node->left->giniIndex << "\t"
		<< node->right->entryIds.size() << ":" << node->right->giniIndex << endl;


	// 3. trainTree left and right
	trainTree(node->right);
	trainTree(node->left); 
}

vector<DataEntry> DecisionTree::loadTestingDataFromFile(string path) {
	cout << "loading testing data from file " << path << "..." << endl;
	vector<DataEntry> tde;
	string csvLine;
	ifstream fin = ifstream(path);

	getline(fin, csvLine);  // 第一行是title

	while (getline(fin, csvLine))
		tde.push_back(DataEntry(csvLine));

	cout << "done." << endl;
	return tde;
}

int DecisionTree::judgeOne(DataEntry* entry) {
	int label = 0, labels[26] = { 0 };
	DecisionNode* node = rootNode;

	while (node->left && node->right) {
		if (entry->getValAt(node->attrId) < node->attrVal)
			node = node->left;
		else
			node = node->right;
	}
	
	for (int i = 0; i < node->entryIds.size(); i++)
		labels[DecisionNode::trainingData[node->entryIds[i]].getLabel() - 1]++;

	for (int i = 0; i < 26; i++)
		label = (labels[i] > labels[label]) ? i : label;
	label += 1;

	// cout << entry->getId() << "," << label << endl;

	return label;
}

float DecisionTree::judgeAllTrainingData() {
	int label, correctNum = 0, size = DecisionNode::trainingData.size();
	float correctRate;

	for (int i = 0; i < size; i++) {
		label = judgeOne(&DecisionNode::trainingData[i]);
		if (label == DecisionNode::trainingData[i].getLabel())
			correctNum++;
	}

	correctRate = (float)correctNum / size;
	cout << correctNum << " / " << size << " = " << correctRate << endl;

	return correctRate;
}

void DecisionTree::judgeAllTestingData(string path) {
	int label, size = DecisionTree::testingData.size();
	ofstream ofs = ofstream(path);

	ofs << "id,label" << endl;
	for (int i = 0; i < size; i++) {
		label = judgeOne(&DecisionTree::testingData[i]);
		ofs << DecisionTree::testingData[i].getId() << "," << label << endl;
	}

	ofs.close();
}