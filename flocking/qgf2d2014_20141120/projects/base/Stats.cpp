#include "Stats.h"

void Stats::SaveCSV(){
	//lowestLifetime, 
	std::ofstream file;
	file.open("resources/results.csv");
	Sort(lifeTimes);
	file << "LifeTime" << "\n";
	for each (auto &a in lifeTimes)
	{
		file << a << "\n";
	}

	file.close();
}

void Stats::Sort(std::vector<float>& vec)
{
	std::sort(vec.begin(), vec.end());
}