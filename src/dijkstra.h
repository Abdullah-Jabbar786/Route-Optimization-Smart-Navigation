#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include "graph.h"
using namespace std;

class PathResult{
    public:
    vector<string> path;
    int distance;
};

PathResult findShortestPath(const Graph& g, const string& start, const string& end);

