#include "Stats.h"

void Stats::SaveCSV(){
	//lowestLifetime, 
	std::ofstream file;
	file.open("resources/Graph2/results.csv");
	 
	Sort(lifeTimes);
	Sort(speeds);
	Sort(sizes);
	
	file << "LifeTime" << "," << "AverageSpeed" << "," << "Size" << "\n";
	for (int i = 0; i < lifeTimes.size(); i++)
		file << lifeTimes[i] << "," << speeds[i] << "," << sizes[i] << "\n";

	file.flush();
	file.close();
}

void Stats::Sort(std::vector<float>& vec)
{
	std::sort(vec.begin(), vec.end());
}