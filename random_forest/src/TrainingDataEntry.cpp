#include "TrainingDataEntry.h"

TrainingDataEntry::TrainingDataEntry(string csvLine)
	: DataEntry(csvLine.substr(0, csvLine.rfind(sep)))
{
	label = atoi(csvLine.substr(csvLine.rfind(sep) + sep.length(),
		csvLine.length()).c_str());
}

int TrainingDataEntry::getLabel() {
	return label;
}