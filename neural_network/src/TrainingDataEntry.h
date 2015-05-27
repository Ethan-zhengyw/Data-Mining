#ifndef _TRAININGDATAENTRY_H_
#define _TRAININGDATAENTRY_H_

#include "DataEntry.h"

class TrainingDataEntry : public DataEntry
{
public:
	TrainingDataEntry(string csvLine);  // id, val0, val1, ..., val383, reference
	float getReference();
	float getNormalizedRef();
	void setNormalizedRef(float val);

private:
	float reference;
	float normalizedRef;
};

#endif