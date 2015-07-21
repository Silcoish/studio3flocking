#ifndef STATS_H
#define STATS_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

class Stats
{
public:
	std::vector<float> lifeTimes;
	std::vector<float> speeds;
	std::vector<float> sizes;

public:
	static Stats& getInstance()
	{
		static Stats instance;

		return instance;
	}

	void SaveCSV();
	void Sort(std::vector<float>& vec);
private:
	Stats() {}

	Stats(Stats const&) = delete;
	void operator=(Stats const&) = delete;
};

#endif