#include "TrainingDataEntry.h"

TrainingDataEntry::TrainingDataEntry(string csvLine)
	: DataEntry(csvLine.substr(0, csvLine.rfind(sep)))
{
	reference = atof(csvLine.substr(csvLine.rfind(sep) + sep.length(),
		csvLine.length()).c_str());
}

float TrainingDataEntry::getReference() {
	return reference;
}

float TrainingDataEntry::getNormalizedRef() {
	return normalizedRef;
}

void TrainingDataEntry::setNormalizedRef(float val) {
	normalizedRef = val;
}