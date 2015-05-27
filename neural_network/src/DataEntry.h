#ifndef _DATAENTRY_H_
#define _DATAENTRY_H_

#include <string>
using namespace std;

class DataEntry
{
public:                         // id, val0, val1, ..., val383
	DataEntry(string csvLine);  //  0,  0.2,  0.3, ..., 0.2
	int getId();
	float getValAt(int attrId);
	static string sep;
	static const int attrNum = 384;
private:
	int id;
	float vals[384];

};

#endif