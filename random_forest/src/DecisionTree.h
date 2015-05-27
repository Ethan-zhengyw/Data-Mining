#ifndef _DECISIONTREE_H_
#define _DECISIONTREE_H_

#include <vector>
#include <set>
#include "TrainingDataEntry.h"
using namespace std;

struct DecisionNode
{
	int attrId;  // 划分用的属性id
	float attrVal;  // 划分的属性值
	vector<int> entryIds;  // 该节点含有的数据id
	float giniIndex;  // 当前节点的基尼系数，描述混乱程度，越小说明越纯，越好。
	static vector<TrainingDataEntry> trainingData;  // 静态成员训练数据，所有节点通过entryIds可访问

	DecisionNode* left;  // attrId对应的属性值比该attrVal小的在left，大的在right
	DecisionNode* right;

	DecisionNode() : left(NULL), right(NULL) {}
	void calGiniIndex();  // 调用此函数后该节点的基尼系数存在giniIndex中
	static vector<TrainingDataEntry> loadTrainingDataFromFile(string path);
};

class DecisionTree
{
public:
	DecisionTree();
	DecisionNode* rootNode;
	set<int> usedAttrs;

	static int getTreeDepth(DecisionNode* node);
	void trainTree(DecisionNode* node);
	int judgeOne(DataEntry* entry);  // 返回判断出来的entry所属的label
	float judgeAllTrainingData();  // 对TrainingData中的每一条进行测试，并返回正确率
	void judgeAllTestingData(string path); // 对静态成员testingData进行决策，将结果写入文件中

	static vector<DataEntry> testingData;
	static vector<DataEntry> loadTestingDataFromFile(string path);
	static int alpha;  // alpha 代表从候选分割值集合下标处的值每次递增的值
	static int beta;  // beta 代表每个决策节点最多的记录数
	static int seed;
};

#endif