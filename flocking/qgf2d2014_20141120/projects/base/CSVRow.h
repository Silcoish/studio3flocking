#ifndef CSVROW_H
#define CSVROW_H

#include <string>
#include <vector>

class CSVRow
{
public:
	void AddValue(std::string& s);
	std::vector<std::string> mData;

private:

public:
	std::string const& operator[] (std::size_t index) const
	{
		return mData[index];
	}
};

#endif