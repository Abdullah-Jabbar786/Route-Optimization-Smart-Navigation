#pragma once
#include <raylib.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "graph.h"
using namespace std;

class Hotel {
public:
    Hotel();
    ~Hotel();
    void update();
    void draw();

    bool isExitClicked() { return exitClicked; }
    void resetExit() { exitClicked = false; }

private:
    Graph graph;
    unordered_map<string, Vector2> nodePos;
    Texture2D bg;
    Vector2 vehiclePos;

    // Exit Button
    Rectangle exitBtnRect = {20, 20, 100, 40};
    bool exitClicked = false;
};
