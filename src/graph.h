#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <utility>

using namespace std;

class Graph {
private:
    int nodeCount;
    vector<vector<pair<int,int>>> adj;
    unordered_map<string,int> nameToIndex;
    vector<string> indexToName;

public:
    Graph();
    Graph(int n);

    void addNode(const string& name);
    void addEdge(const string& u, const string& v, int weight);
    const unordered_map<string, vector<pair<string,int>>>& getAdjacencyList() const;
};
