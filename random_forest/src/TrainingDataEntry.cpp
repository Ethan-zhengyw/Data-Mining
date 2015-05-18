#include "TrainingDataEntry.h"

TrainingDataEntry::TrainingDataEntry(string csvLine)
	: DataEntry(csvLine.substr(0, csvLine.rfind(',')))
{
	label = atoi(csvLine.substr(csvLine.rfind(',') + 2,
		csvLine.length()).c_str());
}

int TrainingDataEntry::getLabel() {
	return label;
}