#include "food_court.h"

FoodCourt::FoodCourt() : graph(10) {
    bg = LoadTexture("src/graphics/foodcourt.png");
    
    graph.addNode("Entrance");
    graph.addNode("PizzaParadise");
    graph.addNode("FootballGround");
    graph.addNode("WokWonders");
    graph.addNode("FusionBakery");
    graph.addNode("Seating");
    graph.addNode("SushiSpot");
    graph.addNode("BurgerJoint");
    graph.addNode("CoffeePastre");

    graph.addEdge("Entrance", "PizzaParadise", 8);
    graph.addEdge("Entrance", "WokWonders", 3);
    graph.addEdge("Entrance", "FootballGround", 5);
    graph.addEdge("Entrance", "Seating", 8);
    graph.addEdge("Entrance", "FusionBakery", 5);
    graph.addEdge("Entrance", "SushiSpot", 10);
    graph.addEdge("Entrance", "BurgerJoint", 16);
    graph.addEdge("Entrance", "CoffeePastre", 18);

    graph.addEdge("PizzaParadise", "FootballGround", 6);
    graph.addEdge("PizzaParadise", "WokWonders", 5);
    graph.addEdge("PizzaParadise", "FusionBakery", 9);
    graph.addEdge("PizzaParadise", "Seating", 4);
    graph.addEdge("PizzaParadise", "SushiSpot", 13);
    graph.addEdge("PizzaParadise", "BurgerJoint", 8);
    graph.addEdge("PizzaParadise", "CoffeePastre", 12);

    graph.addEdge("FootballGround", "WokWonders", 4);
    graph.addEdge("FootballGround", "FusionBakery", 8);
    graph.addEdge("FootballGround", "Seating", 7);
    graph.addEdge("FootballGround", "SushiSpot", 15);
    graph.addEdge("FootballGround", "BurgerJoint", 12);
    graph.addEdge("FootballGround", "CoffeePastre", 17);

    graph.addEdge("WokWonders", "FusionBakery", 3);
    graph.addEdge("WokWonders", "Seating", 5);
    graph.addEdge("WokWonders", "SushiSpot", 8);
    graph.addEdge("WokWonders", "BurgerJoint", 10);
    graph.addEdge("WokWonders", "CoffeePastre", 12);

    graph.addEdge("FusionBakery", "Seating", 7);
    graph.addEdge("FusionBakery", "SushiSpot", 5);
    graph.addEdge("FusionBakery", "BurgerJoint", 12);
    graph.addEdge("FusionBakery", "CoffeePastre", 14);

    graph.addEdge("Seating", "SushiSpot", 10);
    graph.addEdge("Seating", "BurgerJoint", 5);
    graph.addEdge("Seating", "CoffeePastre", 8);

    graph.addEdge("SushiSpot", "BurgerJoint", 14);
    graph.addEdge("SushiSpot", "CoffeePastre", 10);

    graph.addEdge("BurgerJoint", "CoffeePastre", 6);

    nodePos["Entrance"] = {243, 246};
    nodePos["PizzaParadise"] = {753, 219};
    nodePos["FootballGround"] = {296, 33};
    nodePos["WokWonders"] = {413, 272};
    nodePos["FusionBakery"] = {337, 440};
    nodePos["Seating"] = {767, 328};
    nodePos["SushiSpot"] = {297, 762};
    nodePos["BurgerJoint"] = {872, 589};
    nodePos["CoffeePastre"] = {752, 660};

    vehiclePos = nodePos["Entrance"];
}

FoodCourt::~FoodCourt() {
    if(bg.id != 0) UnloadTexture(bg);
}

void FoodCourt::update() {
    // Exit Button Logic
    if (CheckCollisionPointRec(GetMousePosition(), exitBtnRect)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            exitClicked = true;
        }
    }
}

void FoodCourt::draw() {
    DrawTexture(bg, 0, 0, WHITE);

    // Edges are hidden but exist in graph for Dijkstra
    /*
    const auto& adjList = graph.getAdjacencyList();
    for(auto &pair : adjList){
        string u = pair.first;
        if(!nodePos.count(u)) continue;
        for(auto &neighbor : pair.second){
            string v = neighbor.first;
            if(nodePos.count(v) && u < v){
                DrawLineEx(nodePos[u], nodePos[v], 3, RED);
            }
        }
    }
    */

    for(auto &[name, pos] : nodePos) {
        DrawCircleV(pos, 5, RED);
        
        // Aesthetic Text: Boldish (Shadow) + Bigger
        int fontSize = 16;
        int textX = pos.x - MeasureText(name.c_str(), fontSize)/2;
        int textY = pos.y - 25;
        
        // Shadow/Outline for readability (White halo)
        DrawText(name.c_str(), textX+1, textY+1, fontSize, WHITE);
        DrawText(name.c_str(), textX-1, textY-1, fontSize, WHITE);
        DrawText(name.c_str(), textX+1, textY-1, fontSize, WHITE);
        DrawText(name.c_str(), textX-1, textY+1, fontSize, WHITE);
        
        // Main Text
        DrawText(name.c_str(), textX, textY, fontSize, BLACK);
    }

    // Draw Exit Button
    bool hover = CheckCollisionPointRec(GetMousePosition(), exitBtnRect);
    DrawRectangleRec(exitBtnRect, hover ? Fade(RED, 0.8f) : Fade(RED, 0.6f));
    DrawRectangleLinesEx(exitBtnRect, 2, WHITE);
    DrawText("EXIT", exitBtnRect.x + 25, exitBtnRect.y + 10, 20, WHITE);
}
