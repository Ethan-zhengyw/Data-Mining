#include "DecisionTree.h"

class Forest
{
public:
	Forest(int n = 1);  // n代表随机森林中决策树的数目
	int getTreeNum() { return treeNum; }
	void trainAllTrees(int threadNum);  // 该函数创建多个线程，每个线程都会调用trainTree方法，同时训练多棵树
	int judgeOne(DataEntry* entry, int useTreeNum);  // 使用多棵树进行决策，调用每棵树的judgeOne方法，然后投票
	float judgeAllTrainingData(int useTreeNum);
	void judgeAllTestingData(string path, int useTreeNum);
	static void trainTree(DecisionTree *tree);

	int treeNum;
	vector<DecisionTree> trees;
};