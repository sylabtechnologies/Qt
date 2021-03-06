// STLRead.cpp : parse STL	

#include "stdafx.h"
#include "process.h"

std::ostream& operator<< (std::ostream&, REF(StlTriangle));

int main(int argc, char *argv[])
{
	std::cout << WHOAMI << std::endl;

	if (argc != 2)
	{
		std::cout << "Usage : STLRead filename" << std::endl;
		return -1;
	}

	vector<REF(StlTriangle)> data;
	std::ifstream stlFile(argv[1]);

	int status = readData(stlFile, data);
	if (status != 0)
	{
		std::cout << "error " << status << std::endl;
		return status;
	}

	stlFile.close();
	if (data.empty())
	{
		std::cout << "no data " << std::endl;
		return -1;
	}
	else
		std::cout << data.size() << " facets found" << std::endl;

	vector<bool> duplicates(data.size(), false);
	std::ofstream dupFile("duplicates.stl", std::ios::out | std::ios::trunc);

	int dupCnt = findDuplicates(data, duplicates);
	if (dupCnt > 0)
	{
		for (size_t i = 0; i < data.size(); i++)
		{
			if (!duplicates[i]) continue;
			dupFile << data[i] << std::endl;
		}

		dupFile.flush(); dupFile.close();
	}
	std::cout << dupCnt << " duplicated" << std::endl;

	vector<bool> gaps(data.size(), false);
	std::ofstream gapFile("gaps.stl", std::ios::out | std::ios::trunc);

	int gapCnt = findGaps(data, duplicates, gaps);
	if (gapCnt > 0)
	{
		for (size_t i = 0; i < data.size(); i++)
			if (gaps[i]) gapFile << data[i] << std::endl;

		gapFile.flush(); gapFile.close();
	}
	std::cout << gapCnt << " AWOL" << std::endl;

	return 0;
}	

std::ostream& operator<< (std::ostream& os, REF(StlTriangle) ptr)
{
	os << "facet normal ";
	for (size_t i = 0; i < 3; i++)
	{
		os << ptr->normal[i];
		if (i != 2) os << " ";
	}
	os << std::endl;

	os << "\touter loop" << std::endl;
	for (size_t i = 0; i < 3; i++)
	{
		os << "\t\tvertex ";
		for (size_t j = 0; j < 3; j++)
		{
			os << ptr->vertex[i][j] << " ";
		}
		os << std::endl;
	}

	os << "\tend loop" << std::endl;
	os << "endfacet" << std::endl;

	return os;
}

