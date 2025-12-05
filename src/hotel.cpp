#include "hotel.h"

Hotel::Hotel() : graph(10) {
    // Load background
    bg = LoadTexture("src/graphics/hotel3.png"); 
    
    // Add Nodes
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
    
    // Add Edges
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

    // Set Positions
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
    if(bg.id != 0) UnloadTexture(bg);
}

void Hotel::update() {
    // Exit Button Logic
    if (CheckCollisionPointRec(GetMousePosition(), exitBtnRect))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            exitClicked = true;
        }
    }
}

void Hotel::draw() {
    ClearBackground(BEIGE);
    
    if(bg.id != 0) DrawTexture(bg, 0, 0, WHITE);
    else DrawText("Hotel Background Failed to Load", 10, 10, 20, RED);

    DrawText("HOTEL SUB-ZONE", 350, 50, 30, DARKBLUE);

    // Draw Roads (Edges)
    auto drawRoad = [&](string a, string b)
    {
        if (nodePos.find(a) != nodePos.end() && nodePos.find(b) != nodePos.end()) {
            Vector2 A = nodePos[a];
            Vector2 B = nodePos[b];
            DrawLineEx(A, B, 3, RED); // Thicker line for better visibility
        }
    };

    drawRoad("Entrance", "MainLobbyandReception");
    drawRoad("Entrance", "EntertainmentZone");
    drawRoad("Entrance", "Exit");
    drawRoad("MainLobbyandReception", "GuestRoomsWingA");
    drawRoad("MainLobbyandReception", "PoolandSpaArea");
    drawRoad("MainLobbyandReception", "MarketPlaceLane");
    drawRoad("MainLobbyandReception", "FoodCourtandDrink");
    drawRoad("MarketPlaceLane", "FoodCourtandDrink");
    drawRoad("MarketPlaceLane", "ElectronicsHub");
    drawRoad("MarketPlaceLane", "ConferenceCenter");
    drawRoad("FoodCourtandDrink", "EntertainmentZone");
    drawRoad("FoodCourtandDrink", "ElectronicsHub");
    drawRoad("ElectronicsHub", "ConferenceCenter");
    drawRoad("PoolandSpaArea", "GuestRoomsWingA");
    drawRoad("EntertainmentZone", "Exit");

    // Draw Nodes
    for(auto &[name, pos] : nodePos) {
        DrawCircleV(pos, 10, BLUE);
        
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
