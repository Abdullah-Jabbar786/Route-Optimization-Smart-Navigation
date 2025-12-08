#pragma once
#include <raylib.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "graph.h"
#include "dijkstra.h"
using namespace std;

class ParkingLot {
public:
    ParkingLot();
    ~ParkingLot();
    void update();
    void draw();

    bool isExitClicked() { return exitClicked; }
    void resetExit() { exitClicked = false; }
    Vector2 getVehiclePos() const { return vehiclePos; }

    bool isExitRequested() const { return exitRequested; }

private:
    Graph graph;
    unordered_map<string, Vector2> nodePos;
    Texture2D bg;
    Vector2 vehiclePos;

    // Interactive State
    bool showInfo = false;
    string targetName = "";
    int distance = 0;
    float walkingTime = 0.0f;
    vector<string> currentPath;

    // Parking Logic
    class Slot {
        public:
        Rectangle rect;
        Vector2 parkPos;
        bool occupied;
        bool hover;
    };
    
    vector<Slot> slots;
    bool isMoving = false;
    Vector2 targetPos; // Current waypoint target
    vector<Vector2> pathQueue; // Queue of waypoints
    float speed = 4.0f; // Increased speed slightly for better feel

    // Ending Logic
    bool parked = false;
    float parkTimer = 0.0f;
    bool exitRequested = false;
    
    // Exit Button Logic
    Rectangle exitBtnRect = {20, 20, 100, 40};
    bool exitClicked = false;
};
