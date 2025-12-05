#include "amusement_park.h"
#include <iostream>

AmusementPark::AmusementPark() : graph(10){
    bg = LoadTexture("src/graphics/fasmprk2.png");

    graph.addNode("Entrance");
    graph.addNode("RollerCoaster");
    graph.addNode("FerrisWheel");
    graph.addNode("ObservationTower");
    graph.addNode("WaterSlide");
    graph.addNode("cupride");
    graph.addNode("FoodCourt");
    graph.addNode("DropTower");
    graph.addNode("TrainStationRide");
    graph.addNode("Exit");

    graph.addEdge("Entrance", "FerrisWheel", 10);
    graph.addEdge("Entrance", "ObservationTower", 12);
    graph.addEdge("Entrance", "FoodCourt", 15);
    graph.addEdge("Entrance", "cupride", 18);
    graph.addEdge("Entrance", "WaterSlide", 20);
    graph.addEdge("Entrance", "RollerCoaster", 25);
    graph.addEdge("Entrance", "DropTower", 22);
    graph.addEdge("Entrance", "TrainStationRide", 30);
    graph.addEdge("Entrance", "Exit", 35);

    graph.addEdge("FerrisWheel", "ObservationTower", 6);
    graph.addEdge("FerrisWheel", "FoodCourt", 10);
    graph.addEdge("FerrisWheel", "cupride", 12);
    graph.addEdge("FerrisWheel", "WaterSlide", 16);
    graph.addEdge("FerrisWheel", "RollerCoaster", 20);
    graph.addEdge("FerrisWheel", "DropTower", 22);
    graph.addEdge("FerrisWheel", "TrainStationRide", 25);
    graph.addEdge("FerrisWheel", "Exit", 30);

    graph.addEdge("ObservationTower", "FoodCourt", 8);
    graph.addEdge("ObservationTower", "cupride", 14);
    graph.addEdge("ObservationTower", "WaterSlide", 15);
    graph.addEdge("ObservationTower", "RollerCoaster", 18);
    graph.addEdge("ObservationTower", "DropTower", 20);
    graph.addEdge("ObservationTower", "TrainStationRide", 26);
    graph.addEdge("ObservationTower", "Exit", 32);

    graph.addEdge("FoodCourt", "cupride", 5);
    graph.addEdge("FoodCourt", "WaterSlide", 10);
    graph.addEdge("FoodCourt", "RollerCoaster", 15);
    graph.addEdge("FoodCourt", "DropTower", 16);
    graph.addEdge("FoodCourt", "TrainStationRide", 18);
    graph.addEdge("FoodCourt", "Exit", 20);

    graph.addEdge("cupride", "WaterSlide", 8);
    graph.addEdge("cupride", "RollerCoaster", 12);
    graph.addEdge("cupride", "DropTower", 14);
    graph.addEdge("cupride", "TrainStationRide", 16);
    graph.addEdge("cupride", "Exit", 18);

    graph.addEdge("WaterSlide", "RollerCoaster", 10);
    graph.addEdge("WaterSlide", "DropTower", 15);
    graph.addEdge("WaterSlide", "TrainStationRide", 20);
    graph.addEdge("WaterSlide", "Exit", 25);

    graph.addEdge("RollerCoaster", "DropTower", 10);
    graph.addEdge("RollerCoaster", "TrainStationRide", 14);
    graph.addEdge("RollerCoaster", "Exit", 18);

    graph.addEdge("DropTower", "TrainStationRide", 8);
    graph.addEdge("DropTower", "Exit", 12);

    graph.addEdge("TrainStationRide", "Exit", 10);

    nodePos["Entrance"] = {783, 326};
    nodePos["FerrisWheel"] = {198, 189};
    nodePos["ObservationTower"] = {84, 605};
    nodePos["FoodCourt"] = {364, 561};
    nodePos["cupride"] = {645, 528};
    nodePos["WaterSlide"] = {427, 234};
    nodePos["RollerCoaster"] = {532, 140};
    nodePos["DropTower"] = {727, 402};
    nodePos["TrainStationRide"] = {563, 741};
    nodePos["Exit"] = {67, 437};

    vehiclePos = nodePos["Entrance"];
}

AmusementPark::~AmusementPark()
{
    UnloadTexture(bg);
}

void AmusementPark::update()
{
    // Exit Button Logic
    if (CheckCollisionPointRec(GetMousePosition(), exitBtnRect))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            exitClicked = true;
        }
    }
}

void AmusementPark::draw()
{
    DrawTexture(bg, 0, 0, WHITE);

    // Draw edges - HIDDEN for aesthetics but logic remains
    /*
    const auto& adjList = graph.getAdjacencyList();
    for(auto &pair : adjList){
        string u = pair.first;
        if(!nodePos.count(u)) continue;
        for(auto &neighbor : pair.second){
            string v = neighbor.first;
            if(nodePos.count(v) && u < v){ // Draw once
                DrawLineEx(nodePos[u], nodePos[v], 3, RED);
            }
        }
    }
    */

    // Draw nodes
    for (auto &[name, pos] : nodePos)
    {
        // DrawCircleV(pos, 10, LIGHTGRAY); // Optional: Hide dots too if they want just the map? User said "names of the nodes", didn't explicitly say hide dots. I'll keep dots small or maybe just text.
        // Let's keep the dots for now as click targets/visual anchors, but maybe make them subtle or just keep them as is.
        DrawCircleV(pos, 5, RED); // Smaller, distinct

        // Aesthetic Text: Boldish (Shadow) + Bigger
        int fontSize = 16;
        int textX = pos.x - MeasureText(name.c_str(), fontSize) / 2;
        int textY = pos.y - 25;

        // Shadow/Outline for readability (White halo)
        DrawText(name.c_str(), textX + 1, textY + 1, fontSize, WHITE);
        DrawText(name.c_str(), textX - 1, textY - 1, fontSize, WHITE);
        DrawText(name.c_str(), textX + 1, textY - 1, fontSize, WHITE);
        DrawText(name.c_str(), textX - 1, textY + 1, fontSize, WHITE);

        // Main Text
        DrawText(name.c_str(), textX, textY, fontSize, BLACK);
    }

    // Draw Exit Button
    bool hover = CheckCollisionPointRec(GetMousePosition(), exitBtnRect);
    DrawRectangleRec(exitBtnRect, hover ? Fade(RED, 0.8f) : Fade(RED, 0.6f));
    DrawRectangleLinesEx(exitBtnRect, 2, WHITE);
    DrawText("EXIT", exitBtnRect.x + 25, exitBtnRect.y + 10, 20, WHITE);
}
