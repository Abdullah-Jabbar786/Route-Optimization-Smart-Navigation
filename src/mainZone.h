#pragma once
#include <raylib.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <stack> 
#include "graph.h"
#include "dijkstra.h"
#include "astar.h"

using namespace std;

// Stack for Travel History
struct TravelState {
    string placeName;
    float cumulativeDistance;
    float cumulativeTime;
    int depth; // 0=Root(Entrance), 1=MainNodes, 2=SubNodes
};

class MainZone{
public:
    MainZone();
    void update();
    void draw();
    bool isLoadingComplete() const { return loadingComplete; }
    string getEnteredSubMap() const { return enteringSubmap; }
    void resetLoading();
    Vector2 getVehiclePos() const { return vehiclePos; }
    vector<TravelState> getTravelHistory() const;
    void pushExternalHistory(string place, float distDelta, float timeDelta);

private:
    bool loadingComplete = false;
    struct SearchBar{
        Rectangle box = {20,20,250,40};
        char text[50] = "";
        int letterCount = 0;
        bool active = false;
        bool searchTriggered = false;
    } searchBar;

    vector<string> currentPath;
    int pathIndex = 0;
    int travelDistance = 0;
    float vehicleSpeedUnitsPerFrame = 5.0f;
    float timeTaken = 0.0f;
    bool vehicleMoving = false;
    Vector2 vehiclePos = {0,0};
    Vector2 targetPos = {0,0};
    string targetNode = "";
    string startNode = "Entrance";
    bool useAStar = false; // Toggle for A*

    bool showLoading = false;
    int loadingTimer = 0;
    const int loadingDuration = 180;
    int postLoadingTimer = 0; 
    string enteringSubmap = "";
    
    // Teleport Delay Logic
    bool waitingForTeleport = false;
    int teleportDelayTimer = 0;
    
    // Loading State
    float loadingAlpha = 0.0f;
    float loadingBarWidth = 0.0f;
    string loadingStatusText = "";
    int dotTimer = 0;

    unordered_map<string, Vector2> nodePos;
    Graph graph;

    // Travel History Stack
    stack<TravelState> historyStack;

    void updateSearchBar();
    void drawSearchBar();
    void drawNavigationInfo();
    void drawLoadingScreen();
    PathResult getShortestPath(const string& start, const string& end);
    void startNavigation(string target);
    void updateVehicleMovement();
    string getNodeAtPosition(Vector2 pos);
};
