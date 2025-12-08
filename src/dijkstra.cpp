#include "dijkstra.h"
#include <queue>
#include <limits>
#include <algorithm>
#include <iostream>

using Pair = pair<int,string>;

PathResult findShortestPath(const Graph& g, const string& start, const string& end){
    PathResult result;
    unordered_map<string,int> dist;
    unordered_map<string,string> prev;
    priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
    const auto& adjList = g.getAdjacencyList();

    for(auto &p : adjList) dist[p.first] = numeric_limits<int>::max();
    if(!adjList.count(start)){ result.distance = -1; return result; }

    dist[start] = 0;
    pq.push({0,start});

    while(!pq.empty()){
        auto [d,u] = pq.top(); pq.pop();
        if(d > dist[u]) continue;
        for(auto &[v,w] : adjList.at(u)){
            if(dist[u] + w < dist[v]){
                dist[v] = dist[u]+w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if(dist.find(end) == dist.end() || dist[end] == numeric_limits<int>::max()){ result.distance = -1; return result; }

    result.distance = dist[end];
    string cur = end;
    int safety = 0;
    while(cur != start){ 
        result.path.push_back(cur); 
        if(prev.find(cur) == prev.end()) {
             // Path broken
             result.distance = -1; 
             result.path.clear();
             return result;
        }
        cur = prev[cur];
        if(++safety > (int)adjList.size() + 100) break; // Emergency break
    }
    result.path.push_back(start);
    reverse(result.path.begin(), result.path.end());
    return result;
}
