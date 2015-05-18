#include <fstream>
#include "DataEntry.h"

const char sep = ',';
const static int attrNum = 617;

DataEntry::DataEntry(string csvLine)
{
	int i, start = 0, end = csvLine.find(sep);
	id = atoi(csvLine.substr(start, end - start).c_str());

	for (i = 0; i < attrNum - 1; i++) {
		start = end + 2;
		end = csvLine.find(',', start);
		vals[i] = (float)atof(csvLine.substr(start, end - start).c_str());
	}

	start = end + 2;
	end = csvLine.length();
	vals[i] = (float)atof(csvLine.substr(start, end - start).c_str());
}

int DataEntry::getId() {
	return id;
}

float DataEntry::getValAt(int index) {
	return vals[index];
}