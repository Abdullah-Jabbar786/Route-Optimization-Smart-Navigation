#include "food_court.h"
#include "mainZone.h"
#include <cmath>

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
    if (bg.id != 0) UnloadTexture(bg);
}

void FoodCourt::update(MainZone& mz) {
    // Exit Button Logic
    if (CheckCollisionPointRec(GetMousePosition(), exitBtnRect)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            exitClicked = true;
        }
    }

    // Node Click Logic
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (auto &[name, pos] : nodePos) {
            if (CheckCollisionPointCircle(GetMousePosition(), pos, 20)) {
                if (name != "Entrance") {
                    PathResult r = findShortestPath(graph, "Entrance", name);
                    if (r.distance != -1) {
                        distance = r.distance;
                        walkingTime = (float)distance / 1.0f / 3.0f; // Mins
                        targetName = name;
                        currentPath = r.path;
                        showInfo = true;

                        // Push to Main Stack
                        mz.pushExternalHistory(name, (float)distance, walkingTime);

                        // Instant Traversal
                        vehiclePos = nodePos[name];
                    }
                }
            }
        }
    }
}

void FoodCourt::draw() {
    DrawTexture(bg, 0, 0, WHITE);


    for (auto &[name, pos] : nodePos) {
        DrawCircleV(pos, 5, RED);

        // Text: Boldish (Shadow) + Bigger
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

    // Draw Path Highlight
    if (!currentPath.empty()) {
        for (size_t i = 0; i < currentPath.size() - 1; i++) {
            if (nodePos.count(currentPath[i]) && nodePos.count(currentPath[i + 1])) {
                DrawLineEx(nodePos[currentPath[i]], nodePos[currentPath[i + 1]], 6, LIME);
            }
        }
    }

    // Draw Info Box
    if (showInfo) {
        int w = 250, h = 100;
        int x = GetScreenWidth() - w - 20, y = GetScreenHeight() - h - 20;
        DrawRectangle(x, y, w, h, Fade(SKYBLUE, 0.9f));
        DrawRectangleLines(x, y, w, h, DARKBLUE);

        DrawText("TARGET INFO", x + 10, y + 10, 24, DARKBLUE);
        DrawText(("To: " + targetName).c_str(), x + 10, y + 35, 18, BLACK);
        DrawText(("Dist: " + to_string(distance) + " m").c_str(), x + 10, y + 55, 22, BLACK);

        string tStr = (walkingTime < 1.0f) ? "< 1 min" : to_string((int)ceil(walkingTime)) + " mins";
        DrawText(("Time: " + tStr).c_str(), x + 10, y + 75, 22, DARKGREEN);
    }

    // Draw Exit Button
    bool hover = CheckCollisionPointRec(GetMousePosition(), exitBtnRect);
    DrawRectangleRec(exitBtnRect, hover ? Fade(RED, 0.8f) : Fade(RED, 0.6f));
    DrawRectangleLinesEx(exitBtnRect, 2, WHITE);
    DrawText("EXIT", exitBtnRect.x + 25, exitBtnRect.y + 10, 20, WHITE);
}
