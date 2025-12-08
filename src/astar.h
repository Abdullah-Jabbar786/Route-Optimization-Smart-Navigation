#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <raylib.h> // For Vector2
#include "graph.h"
#include "dijkstra.h" // Reuse PathResult
using namespace std;

PathResult findShortestPathAStar(const Graph& g, const string& start, const string& end, const unordered_map<string, Vector2>& positions);
