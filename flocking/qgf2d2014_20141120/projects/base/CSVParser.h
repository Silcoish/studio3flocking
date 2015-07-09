#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "CSVRow.h"

class CSVParser
{
private:
	std::string fileLocation;

public:
	std::vector<CSVRow*> mRows;
	enum returnType
	{
		FLOAT,
		STRING
	};

public:
	CSVParser();
	~CSVParser();

	bool LoadFromFile(const std::string& fl); //returns if it was successful
	float GetDataFloat(int index, int& value);
	std::string& GetDataString(int index, int value);
};

#endif CSVPARSER_H