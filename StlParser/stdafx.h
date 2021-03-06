// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>
#include <unordered_map>
#include <set>
#include <algorithm>

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;

#include "StlTriangle.h"

#define REF(D) shared_ptr<D>	// make Java refs
#define MAKEREF(X, D, ...) auto X {make_shared<D>( ##__VA_ARGS__## )}

#define WHOAMI "STL Reader v1.0"
