#include "astar.h"
#include <queue>
#include <limits>
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;

// Pair of (f_score, node_name)
using AStarPair = pair<double, string>;

double calculateHeuristic(const string &u, const string &target, const unordered_map<string, Vector2> &positions)
{
    if (positions.count(u) && positions.count(target))
    {
        Vector2 p1 = positions.at(u);
        Vector2 p2 = positions.at(target);
        return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    }
    return 0.0;
}

PathResult findShortestPathAStar(const Graph &g, const string &start, const string &end, const unordered_map<string, Vector2> &positions)
{
    PathResult result;
    unordered_map<string, double> g_score;
    unordered_map<string, double> f_score;
    unordered_map<string, string> prev;
    priority_queue<AStarPair, vector<AStarPair>, greater<AStarPair>> pq;

    const auto &adjList = g.getAdjacencyList();

    for (auto &p : adjList)
    {
        g_score[p.first] = numeric_limits<double>::max();
        f_score[p.first] = numeric_limits<double>::max();
    }

    if (!adjList.count(start))
    {
        result.distance = -1;
        return result;
    }

    g_score[start] = 0;
    f_score[start] = calculateHeuristic(start, end, positions);
    pq.push({f_score[start], start});

    while (!pq.empty())
    {
        auto [f, u] = pq.top();
        pq.pop();
        if (u == end)
            break; // Found target
        if (f > f_score[u])
            continue;

        for (auto &[v, w] : adjList.at(u))
        {
            double tentative_g = g_score[u] + w;
            if (tentative_g < g_score[v])
            {
                prev[v] = u;
                g_score[v] = tentative_g;
                f_score[v] = g_score[v] + calculateHeuristic(v, end, positions);
                pq.push({f_score[v], v});
            }
        }
    }

    if (g_score[end] == numeric_limits<double>::max())
    {
        result.distance = -1;
        return result;
    }

    result.distance = (int)g_score[end]; // Cast back to int for compatibility
    string cur = end;
    while (cur != start)
    {
        result.path.push_back(cur);
        cur = prev[cur];
    }
    result.path.push_back(start);
    reverse(result.path.begin(), result.path.end());
    return result;
}
