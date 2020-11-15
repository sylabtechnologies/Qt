#pragma once
#include <memory>
using std::shared_ptr;

class StlTriangle
{
	int compare(float *a, float *b)
	{
		for (size_t i = 0; i < 3; i++)
		{
			if (a[i] > b[i])
				return 1;
			else if (a[i] < b[i])
				return -1;
		}

		return 0;
	}

public:
	float normal[3]{};
	float vertex[3][3]{};

	StlTriangle(float *data);

	void getMidvec(float*);
	void getSide(int, float*);
	int  match(shared_ptr<StlTriangle>);
};

