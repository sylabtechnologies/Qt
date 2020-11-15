#include "stdafx.h"

// sort by x, y, z
StlTriangle::StlTriangle(float *arr)
{
	for (size_t i = 0; i < 3; i++)
		normal[i] = arr[i];

	float *pos0{ arr + 3}, *pos1{ arr + 6 }, *pos2{ arr + 9 };
	vector<float*> order;
	order.push_back(pos0);

	if (compare(pos0, pos1) > 0)
		order.insert(order.begin(), pos1);
	else
		order.push_back(pos1);

	auto iter = order.begin();
	if (compare(pos1, pos2) > 0)
	{
		if (compare(pos0, pos2) <= 0)
			iter++;

		order.insert(iter, pos2);
	}
	else
		order.push_back(pos2);

	iter = order.begin();
	for (size_t i = 0; i < 3; i++)
	{
		auto ptr = *iter++;
		for (size_t j = 0; j < 3; j++)
			vertex[i][j] = ptr[j];
	}
}

void StlTriangle::getMidvec(float *ans)
{
	for (size_t i = 0; i < 3; i++)
	{
		ans[i] = 0.0;
		for (size_t j = 0; j < 3; j++)
			ans[i] += vertex[j][i] / 3;
	}
}

void StlTriangle::getSide(int side, float *res)
{
	int a = side;
	int b = side + 1;
	if (b >= 3) b = 0;

	for (size_t i = 0; i < 3; i++)
		res[i] = (vertex[a][i] + vertex[b][i]) / 2;
}

int StlTriangle::match(const shared_ptr<StlTriangle> ptr)
{
	int count = 0;
	for (size_t i = 0; i < 3; i++)
	{
		float *A = vertex[i];
		bool found = false;

		for (size_t j = 0; j < 3; j++)
		{
			float *B = ptr->vertex[j];
			int cmp = compare(A, B);

			if (cmp > 0) continue;

			if (cmp == 0)
			{
				found = true;
				break;
			}

		}

		if (found) count++;
	}

	return count;
}
