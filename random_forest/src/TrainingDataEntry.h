#ifndef _TRAININGDATAENTRY_H_
#define _TRAININGDATAENTRY_H_

#include "DataEntry.h"

class TrainingDataEntry : public DataEntry
{
public:
	TrainingDataEntry(string csvLine);  // id, val0, val1, ..., val616, label
	int getLabel();
private:
	int label;
};

#endif