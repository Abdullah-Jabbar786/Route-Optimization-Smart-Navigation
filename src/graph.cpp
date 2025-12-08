#include "graph.h"

Graph::Graph() : nodeCount(0) {}
Graph::Graph(int n) : nodeCount(0) { adj.resize(n); }

void Graph::addNode(const string& name) {
    if (!nameToIndex.count(name)) {
        nameToIndex[name] = nodeCount++;
        indexToName.push_back(name);
        adj.resize(nodeCount);
    }
}

void Graph::addEdge(const string& u, const string& v, int weight) {
    int a = nameToIndex[u];
    int b = nameToIndex[v];
    adj[a].push_back({b, weight});
    adj[b].push_back({a, weight}); 
}

const unordered_map<string, vector<pair<string,int>>>& Graph::getAdjacencyList() const {
    static unordered_map<string, vector<pair<string,int>>> list; // Kept static for performance.

    list.clear();
    for(int i=0; i<nodeCount; i++){
        string name = indexToName[i];
        for(auto &p : adj[i]){
            string neighbor = indexToName[p.first];
            int w = p.second;
            list[name].push_back({neighbor,w});
        }
    }
    return list;
}
