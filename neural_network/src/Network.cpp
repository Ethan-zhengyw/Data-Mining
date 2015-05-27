#include "Network.h"
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

Network::Network(int input, int hidden, int output, float alpha_, float beta_) {
	iN = input;
	hN = hidden;
	oN = output;
	alpha = alpha_;
	beta = beta_;
	initWeight();
}

void Network::loadDataFromFile(string path, DataType dataType) {
	if (dataType == DataType::TRAIN)
		cout << "loading traing data from file " << path << "..." << endl;
	else 
		cout << "loading testing data from file " << path << "..." << endl;

	string csvLine;
	ifstream fin = ifstream(path);

	getline(fin, csvLine);  // 第一行是title

	if (dataType == DataType::TRAIN) {
		int i = 0;
		float ref;
		minRef = 10000.0f, maxRef = 0.0f;
		while (getline(fin, csvLine)) {
			trainingData.push_back(TrainingDataEntry(csvLine));
			ref = trainingData[i++].getReference();
			minRef = (ref < minRef) ? ref : minRef;
			maxRef = (ref > maxRef) ? ref : maxRef;
		}
		for (int i = 0; i < trainingData.size(); i++) {
			trainingData[i].setNormalizedRef((trainingData[i].getReference() - minRef) / (maxRef - minRef));
		}

	} else if (dataType == DataType::TEST){
		while (getline(fin, csvLine))
			testingData.push_back(DataEntry(csvLine));
	}

	cout << "done." << endl;
}

// initialize all weight in range -0.2 to 0.2
void Network::initWeight(int seed) {
	ihW.clear(); ihC.clear();
	hoW.clear(); hoC.clear();
	hE.clear(); oE.clear();
	iO.clear(); hE.clear(); oE.clear();
	
	srand((unsigned int)time(NULL) + seed);

	// initialize weights of the input-hidden layer
	for (int i = 0; i < iN; i++) {
		ihW.push_back(vector<float>());
		ihC.push_back(vector<float>());
		for (int j = 0; j < hN; j++) {
			ihW[i].push_back(float(rand() % 4000) / 1000 - 2);
			ihC[i].push_back(0);
		}
	}

	// hidden-output layer
	for (int i = 0; i < hN; i++) {
		hoW.push_back(vector<float>());
		hoC.push_back(vector<float>());
		for (int j = 0; j < oN; j++) {
			hoW[i].push_back(float(rand() % 4000) / 1000 - 2);
			hoC[i].push_back(0);
		}
	}

	for (int i = 0; i < iN; i++) {
		iO.push_back(0);
	}

	for (int i = 0; i < hN; i++) {
		hE.push_back(0);
		hO.push_back(0);
	}
	for (int i = 0; i < oN; i++) {
		oE.push_back(0);
		oO.push_back(0);
	}
}

void Network::feedForward(TrainingDataEntry entry) {
	float result;

	for (int i = 0; i < iN; i++) {
		iO[i] = entry.getValAt(i);
	}

	// calculate the output of hidden layer
	for (int i = 0; i < hN; i++) {
		result = 0;
		for (int j = 0; j < iN; j++) {
			result += entry.getValAt(j) * ihW[j][i];
		}
		result = sigmoid(result);
		hO[i] = result;
	}

	// calculate the result of output layer
	for (int i = 0; i < oN; i++) {
		result = 0;
		for (int j = 0; j < hN; j++) {
			result += hO[j] * hoW[j][i];
		}
		result = sigmoid(result);
		oO[i] = result;
	}
}

// feedForward should be call before calling this function
void Network::backPropagation(float desired) {
	// error of output layer and output layer
	// only one output now, so the deisred is a number, when there are more than one output
	// just replace the parameter desired with a float vector
	oE[0] = oO[0] * (1 - oO[0]) * (desired - oO[0]);
	for (int i = 0; i < hN; i++) {
		hE[i] = oE[0] * hoW[i][0];
	}


	float tempWeightChange;

	// update hidden-output layer's weight
	for (int i = 0; i < hN; i++) {
		for (int j = 0; j < oN; j++) {
			tempWeightChange = beta * oE[j] * hO[i];
			hoW[i][j] = hoW[i][j] + tempWeightChange + (alpha * hoC[i][j]);
			hoC[i][j] = tempWeightChange;
		}
	}

	// update input-hidden layer's weight
	for (int i = 0; i < iN; i++) {
		for (int j = 0; j < hN; j++) {
			tempWeightChange = beta * hE[j] * iO[i];
			ihW[i][j] = ihW[i][j] + tempWeightChange + (alpha * ihC[i][j]);
			ihC[i][j] = tempWeightChange;
		}
	}
}

float Network::sigmoid(float x) {
	return 1.0f / (1.0f + exp(-x));
}

void Network::changeWeight() {
	ihW[0][0] = 0.4f;
	ihW[0][1] = -0.1f;
	ihW[1][0] = 0.1f;
	ihW[1][1] = -0.1f;

	hoW[0][0] = 0.06f;
	hoW[1][0] = -0.4f;
}

float Network::judgeOne(DataEntry entry) {
	float result;

	// calculate the output of hidden layer
	for (int i = 0; i < hN; i++) {
		result = 0;
		for (int j = 0; j < iN; j++) {
			result += entry.getValAt(j) * ihW[j][i];
		}
		result = sigmoid(result);
		hO[i] = result;
	}

	// calculate the result of output layer
	for (int i = 0; i < oN; i++) {
		result = 0;
		for (int j = 0; j < hN; j++) {
			result += hO[j] * hoW[j][i];
		}
		result = sigmoid(result);
		oO[i] = result;
	}

	return oO[0] * (maxRef - minRef) + minRef;
}

float Network::judgeAllTrainingData() {
	int size = trainingData.size();
	float ref, desired_ref, errorSum = 0;

	for (int i = 0; i < size; i++) {
		ref = judgeOne(trainingData[i]);
		desired_ref = trainingData[i].getReference();
		errorSum += abs(ref - desired_ref);
	}

	cout << preScore << "-->";
	preScore = curScore;
	curScore = errorSum / size;
	cout << curScore << " ^ " << preScore - curScore << endl;


	if (preScore < curScore) {
		cout << "alpha: " << alpha << " beta: " << beta << endl;
		cout << "update alpha: ";
		cin >> alpha;
		cout << "update beta: ";
		cin >> beta;

		if (curScore < bestScore) {
			cout << "curScore: " << curScore << endl;
			// judgeAllTestingData("result_best.csv");
		}
	}
	bestScore = (curScore < bestScore) ? curScore : bestScore;

	return curScore;
}

// parameter path is where the result write
void Network::judgeAllTestingData(string path) {
	int size = testingData.size();
	float ref;
	ofstream ofs = ofstream(path);

	ofs << "id,reference" << endl;
	for (int i = 0; i < size; i++) {
		ref = judgeOne(testingData[i]);
		ofs << testingData[i].getId() << "," << ref << endl;
	}

	ofs.close();
}

void Network::setAlpha(float alpha_) {
	alpha = alpha_;
}

float Network::getAlpha() {
	return alpha;
}

void Network::setBeta(float beta_) {
	beta = beta_;
}

float Network::getBeta() {
	return beta;
}

void Network::trainOne(TrainingDataEntry entry) {
	oE[0] = 100;
	float maxError = 0.05f;
	while ((oE[0] > maxError) || (oE[0] < -maxError)) {
		feedForward(entry);
		backPropagation(entry.getNormalizedRef());
		// cout << "oE: " << oE[0] << "max: " << maxError << "min: " << -maxError << endl;
	}
	// cout << "done oE: " << i << endl;
}

void Network::trainNetwork() {
	for (int i = 0; i < trainingData.size(); i++)
		trainOne(trainingData[i]);
}