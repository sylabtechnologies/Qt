#pragma once
#include "stdafx.h"

int readData(std::ifstream&, vector<REF(StlTriangle)>&);
int findDuplicates(const vector<REF(StlTriangle)>&, vector<bool>&);
int findGaps(const vector<REF(StlTriangle)>&, const vector<bool>&, vector<bool>&);

