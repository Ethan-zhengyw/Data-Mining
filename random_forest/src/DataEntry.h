#ifndef _DATAENTRY_H_
#define _DATAENTRY_H_

#include <string>
using namespace std;

class DataEntry
{
public:                         // id, val0, val1, ..., val617
	DataEntry(string csvLine);  //  0,  0.2,  0.3, ..., 0.2
	~DataEntry();
	int getId();
	float getValAt(int attrId);
	static string sep;
	static const int attrNum = 617;
	static const int labelNum = 26;
private:
	int id;
	float vals[617];

};

#endif