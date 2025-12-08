#include <raylib.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "graph.h"
#include "dijkstra.h"
using namespace std;

class MainZone; 
class FoodCourt{
public:
    FoodCourt();
    ~FoodCourt();
    void update(MainZone& mz);
    void draw();

    bool isExitClicked() { return exitClicked; }
    void resetExit() { exitClicked = false; }
    Vector2 getVehiclePos() const { return vehiclePos; }

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

    // Exit Button Logic
    Rectangle exitBtnRect = {20, 20, 100, 40};
    bool exitClicked = false;
};
