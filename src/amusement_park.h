#pragma once
#include <raylib.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "graph.h"
using namespace std;

class AmusementPark {
public:
    AmusementPark();
    ~AmusementPark();
    void update();
    void draw();

    bool isExitClicked() { return exitClicked; }
    void resetExit() { exitClicked = false; }

private:
    Graph graph;
    unordered_map<string, Vector2> nodePos;
    Texture2D bg;
    
    // Simple vehicle for sub-zone navigation if needed later
    Vector2 vehiclePos;

    // Exit Button Logic
    Rectangle exitBtnRect = {20, 20, 100, 40}; // Top-left corner
    bool exitClicked = false;
};
