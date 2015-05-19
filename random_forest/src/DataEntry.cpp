#include <fstream>
#include "DataEntry.h"

string DataEntry::sep = ",";

DataEntry::DataEntry(string csvLine)
{
	int i, start = 0, end = csvLine.find(sep);
	id = atoi(csvLine.substr(start, end - start).c_str());

	for (i = 0; i < attrNum - 1; i++) {
		start = end + sep.length();
		end = csvLine.find(sep, start);
		vals[i] = (float)atof(csvLine.substr(start, end - start).c_str());
	}

	start = end + sep.length();
	end = csvLine.length();
	vals[i] = (float)atof(csvLine.substr(start, end - start).c_str());
}

int DataEntry::getId() {
	return id;
}

float DataEntry::getValAt(int index) {
	return vals[index];
}

DataEntry::~DataEntry() {
	// delete[] vals;
}