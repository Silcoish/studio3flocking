#include "CSVParser.h"

CSVParser::CSVParser()
{

}

CSVParser::~CSVParser()
{

}

bool CSVParser::LoadFromFile(const std::string& fl)
{
	fileLocation = fl;
	std::string line;
	std::ifstream dataFile (fileLocation);

	if (dataFile.is_open())
	{
		int loopNum = 0;
		while (std::getline(dataFile, line))
		{
			loopNum++;
			
			if (loopNum == 1 || loopNum == 2)
				continue;

			if (line[0] == ',')
				continue;

			mRows.push_back(new CSVRow());

			std::size_t len = line.size();
			std::size_t pos = line.find(',');

			while (pos < len)
			{
				mRows[loopNum - 3]->AddValue(line.substr(0, pos));
				line = line.substr(pos + 1);
				pos = line.find(',');

				if (pos > len)
				{
					mRows[loopNum - 3]->AddValue(line);
				}
			}
		}

		std::cout << "Finished Parsing File" << std::endl;

		dataFile.close();

		return true;
	}

	return false;
}

float CSVParser::GetDataFloat(int index, int& value)
{
	value++;
	return std::stof(mRows[index]->mData[value - 1]);
}

std::string CSVParser::GetDataString(int index, int& value)
{
	value++;
	return mRows[index]->mData[value - 1];
}

void CSVParser::Unload()
{
	for (auto &a : mRows)
	{
		a->mData.clear();
	}

	mRows.clear();

	std::cout << "Cleaned up parser memory" << std::endl;
}