#include <fstream>
#include <iostream>
#include "DecisionTree.h"

vector<TrainingDataEntry> DecisionTree::trainingData = DecisionTree::loadTrainingDataFromFile("data/train.csv");

vector<TrainingDataEntry> DecisionTree::loadTrainingDataFromFile(string path) {
	vector<TrainingDataEntry> tde;
	string csvLine;
	ifstream fin = ifstream("data/train_300.csv");

	getline(fin, csvLine);  // 第一行是title

	while (getline(fin, csvLine))
		tde.push_back(TrainingDataEntry(csvLine));

	return tde;
}