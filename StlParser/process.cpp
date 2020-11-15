#include "stdafx.h"

float globalMinCoord = INT32_MAX;
float globalMaxCoord = INT32_MIN;
float globalMaxRange = 0.0;

int myHashcode(float *arr)
{
	const int NUMSLICES = 2000;

	int ret = 113;
	for (size_t i = 0; i < 3; i++)
	{
		float delta = (arr[i] - globalMinCoord) / globalMaxRange;
		ret = (ret * 17 + delta * NUMSLICES );
	}

	return ret;
}

int readData(std::ifstream &stlFile, vector<REF(StlTriangle)> &result)
{
	bool ok = false, fin = false;

	const int elemSize = 21;
	string line, tokens[elemSize];

	if (std::getline(stlFile, line))
	{
		std::istringstream iss(line);
		if (iss >> tokens[0] && tokens[0] == "solid")
			ok = true;
	}

	if (!ok) return -1;

	ok = true;
	try
	{
		while (!fin)
		{
			for (size_t i = 0; i < elemSize; i++)
			{
				if (stlFile >> tokens[i])
					if (tokens[i] == "endsolid")
					{
						if (i == 0) fin = true;
						ok = false;
						break;
					}
			}

			if (!ok) break;

			size_t curr = 0;
			if (tokens[curr++] != "facet") break;
			if (tokens[curr++] != "normal") break;

			float mydata[12];
			size_t currData = 0;
			for (size_t j = 0; j < 3; j++)
				mydata[currData++] = std::stof(tokens[curr++]);

			if (tokens[curr++] != "outer") break;
			if (tokens[curr++] != "loop") break;

			for (size_t i = 0; i < 3; i++)
			{
				if (tokens[curr++] != "vertex") break;

				float row[3]{};
				for (size_t j = 0; j < 3; j++)
				{
					float val = std::stof(tokens[curr++]);
					globalMinCoord = std::min(globalMinCoord, val);
					globalMaxCoord = std::max(globalMaxCoord, val);
					mydata[currData++] = val;
				}
			}

			if (tokens[curr++] != "endloop") break;
			if (tokens[curr++] != "endfacet") break;

			result.push_back(make_shared<StlTriangle>(mydata));
		}
	}
	catch (std::invalid_argument &ia)
	{
		std::cerr << " Invalid_argument " << ia.what() << std::endl;
		fin = false;
	}

	globalMaxRange = globalMaxCoord - globalMinCoord;

	if (fin)
		return 0;
	else
		return -1;
}

int findDuplicates(const vector<REF(StlTriangle)> &data, vector<bool> &dups)
{
	int ret{ 0 };
	std::unordered_map<int, REF(StlTriangle)> map;

	float curr[3];
	for (size_t i = 0; i < data.size(); i++)
	{
		if (dups[i]) continue;

		data[i]->getMidvec(curr);
		int key = myHashcode(curr);

		if (map.find(key) != map.end())
		{
			auto itr1 = map.lower_bound(key);
			auto itr2 = map.upper_bound(key);

			while (itr1 != itr2)
			{
				auto test = itr1->second;

				if (test->match(data[i]) == 3)
				{
					dups[i] = true;
					ret++;
				}

				itr1++;
			}

		}
		else
			map.emplace(key, data[i]);
	}

	return ret;
}

int findGaps(const vector<REF(StlTriangle)> &data, const vector<bool> &duplicates, vector<bool> &gaps)
{
	vector<bool> edgeOK(data.size() * 3, false);
	std::unordered_multimap<int, int> map;

	float curr[3];
	for (size_t i = 0; i < data.size(); i++)
	{
		if (duplicates[i]) continue;

		for (size_t side = 0; side < 3; side++)
		{
			data[i]->getSide(side, curr);
			int key = myHashcode(curr);
			map.emplace(key, i);
		}
	}

	for (size_t i = 0; i < data.size(); i++)
	{
		if (duplicates[i]) continue;

		for (size_t side = 0; side < 3; side++)
		{
			data[i]->getSide(side, curr);
			int key = myHashcode(curr);

			if (map.find(key) != map.end())
			{
				auto itr1 = map.lower_bound(key);
				auto itr2 = map.upper_bound(key);

				while (itr1 != itr2)
				{
					if (itr1->second != i)
					{ 
						auto test = data[itr1->second];
						if (test->match(data[i]) == 2)
						{
							edgeOK[i * 3 + side] = true;
							break;
						}
					}

					itr1++;
				}
			}
		}
	}

	int ret{ 0 };
	for (size_t i = 0; i < data.size(); i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			if (!edgeOK[i * 3 + j])
			{
				gaps[i] = true;
				ret++;
				break;
			}
		}
	}

	return ret;
}
