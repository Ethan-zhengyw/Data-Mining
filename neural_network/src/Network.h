#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "TrainingDataEntry.h"
#include <vector>

enum DataType {
	TRAIN, TEST
};

class Network
{
public:
	Network(int input, int hidden, int output, float alpha, float beta);
	void setAlpha(float alpha_);
	float getAlpha();
	void setBeta(float beta_);
	float getBeta();
	void loadDataFromFile(string path, DataType dataType);
	void initWeight(int seed = 0);
	float sigmoid(float x);

	void trainOne(TrainingDataEntry entry);
	void feedForward(TrainingDataEntry entry);
	void backPropagation(float desired);
	void trainNetwork();

	float judgeOne(DataEntry entry);
	float judgeAllTrainingData();
	void judgeAllTestingData(string path);
	void changeWeight();  // for change weight to default value as BPNN paper

	std::vector<DataEntry> testingData;
	std::vector<TrainingDataEntry> trainingData;
private:
	int iN, hN, oN;  // N-Number
	float alpha, beta;
	float minRef, maxRef;

	vector<float> iO, hO, oO;  // temp result in every node
	vector<float> hE, oE;  // error in every node f'(x)*delta(x) Ps: f'(x) = f(x)(1-f(x)) when f is sigmoid
	vector<vector<float>>  ihW, hoW;  // i-input, h-hidden, o-output, W-weight
	vector<vector<float>>  ihC, hoC;  // c-change

	float preScore = 9999, curScore = 9999, bestScore = 9999;
};

#endif
