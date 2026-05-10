#include "hotel.h"
#include "mainZone.h"
#include <cmath>

Hotel::Hotel() : graph(10) {

    bg = LoadTexture("src/graphics/hotel3.png");
    graph.addNode("Entrance");
    graph.addNode("PoolandSpaArea");
    graph.addNode("GuestRoomsWingA");
    graph.addNode("MainLobbyandReception");
    graph.addNode("MarketPlaceLane");
    graph.addNode("FoodCourtandDrink");
    graph.addNode("EntertainmentZone");
    graph.addNode("ElectronicsHub");
    graph.addNode("ConferenceCenter");
    graph.addNode("Exit");


    graph.addEdge("Entrance", "MainLobbyandReception", 12);
    graph.addEdge("Entrance", "EntertainmentZone", 10);
    graph.addEdge("Entrance", "Exit", 25);

    graph.addEdge("MainLobbyandReception", "GuestRoomsWingA", 10);
    graph.addEdge("MainLobbyandReception", "PoolandSpaArea", 14);
    graph.addEdge("MainLobbyandReception", "MarketPlaceLane", 11);
    graph.addEdge("MainLobbyandReception", "FoodCourtandDrink", 8);

    graph.addEdge("MarketPlaceLane", "FoodCourtandDrink", 6);
    graph.addEdge("MarketPlaceLane", "ElectronicsHub", 11);
    graph.addEdge("MarketPlaceLane", "ConferenceCenter", 15);

    graph.addEdge("FoodCourtandDrink", "EntertainmentZone", 9);
    graph.addEdge("FoodCourtandDrink", "ElectronicsHub", 12);
    graph.addEdge("ElectronicsHub", "ConferenceCenter", 7);
    graph.addEdge("PoolandSpaArea", "GuestRoomsWingA", 6);
    graph.addEdge("EntertainmentZone", "Exit", 18);
    graph.addEdge("EntertainmentZone", "ElectronicsHub", 10);

    //Positions
    nodePos["Entrance"] = {451, 525};
    nodePos["PoolandSpaArea"] = {210, 337};
    nodePos["GuestRoomsWingA"] = {259, 239};
    nodePos["MainLobbyandReception"] = {510, 114};
    nodePos["MarketPlaceLane"] = {611, 333};
    nodePos["FoodCourtandDrink"] = {597, 454};
    nodePos["EntertainmentZone"] = {494, 612};
    nodePos["ElectronicsHub"] = {610, 662};
    nodePos["ConferenceCenter"] = {739, 612};
    nodePos["Exit"] = {211, 782};

    vehiclePos = nodePos["Entrance"];
}

Hotel::~Hotel() {
    if (bg.id != 0) UnloadTexture(bg);
}

void Hotel::update(MainZone& mz) {
    //Exit Button Logic
    if (CheckCollisionPointRec(GetMousePosition(), exitBtnRect)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            exitClicked = true;
        }
    }

    //Exit Node Delay Logic
    if (exitNodeSelected) {
        exitNodeTimer += GetFrameTime();
        if (exitNodeTimer >= 5.0f) {
            exitClicked = true;
            exitNodeSelected = false;
            exitNodeTimer = 0.0f;
        }
    }

    // Node Click Logic
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (auto &[name, pos] : nodePos) {
            if (CheckCollisionPointCircle(GetMousePosition(), pos, 20)) {
                if (name != "Entrance") { // Don't path to self
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

                        // Check if Exit Node Clicked
                        if (name == "Exit") {
                            exitNodeSelected = true;
                            exitNodeTimer = 0.0f;
                        } else {
                            exitNodeSelected = false;
                            exitNodeTimer = 0.0f;
                        }
                    }
                }
            }
        }
    }
}

void Hotel::draw() {
    ClearBackground(BEIGE);

    if (bg.id != 0) DrawTexture(bg, 0, 0, WHITE);
    else DrawText("Hotel Background Failed to Load", 10, 10, 20, RED);

    // Draw Nodes
    for (auto &[name, pos] : nodePos) {
        DrawCircleV(pos, 10, BLUE);

        // Text: Boldish (Shadow) + Bigger
        int fontSize = 16;
        int textX = pos.x - MeasureText(name.c_str(), fontSize) / 2;
        int textY = pos.y - 25;

        // Shadow/Outline for readability (White halo)
        DrawText(name.c_str(), textX + 1, textY + 1, fontSize, WHITE);
        DrawText(name.c_str(), textX - 1, textY - 1, fontSize, WHITE);
        DrawText(name.c_str(), textX + 1, textY - 1, fontSize, WHITE);
        DrawText(name.c_str(), textX - 1, textY + 1, fontSize, WHITE);

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
    if(showInfo){
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
