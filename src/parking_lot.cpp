#include "parking_lot.h"

ParkingLot::ParkingLot() : graph(5){
    bg = LoadTexture("src/graphics/pklot1.png");
    
    // Nodes removed for animation phase
    /*
    graph.addNode("Entrance");
    graph.addNode("SectionA");
    graph.addNode("SectionB");
    
    graph.addEdge("Entrance", "SectionA", 4);
    graph.addEdge("Entrance", "SectionB", 4);

    nodePos["Entrance"] = {450, 700};
    nodePos["SectionA"] = {250, 400};
    nodePos["SectionB"] = {650, 400};

    vehiclePos = nodePos["Entrance"];
    */
}

ParkingLot::~ParkingLot() {
    if(bg.id != 0) UnloadTexture(bg);
}

void ParkingLot::update() {
    // Exit Button Logic
    if (CheckCollisionPointRec(GetMousePosition(), exitBtnRect))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            exitClicked = true;
        }
    }
}

void ParkingLot::draw() {
    ClearBackground(DARKGRAY);
    
    if(bg.id != 0) DrawTexture(bg, 0, 0, WHITE);
    else DrawText("PARKING LOT SUB-ZONE (Image Not Found)", 300, 50, 30, WHITE);

    /*
    for(auto &[name, pos] : nodePos) {
        DrawCircleV(pos, 10, YELLOW);
        
        // Aesthetic Text: Boldish (Shadow) + Bigger
        int fontSize = 16;
        int textX = pos.x - MeasureText(name.c_str(), fontSize)/2;
        int textY = pos.y - 25;
        
        // Shadow/Outline for readability (White halo)
        DrawText(name.c_str(), textX+1, textY+1, fontSize, BLACK); // Black shadow for yellow dots? Or White? Let's stick to white halo for consistency or black if background is light. Parking lot bg is DARKGRAY. So White halo is good.
        DrawText(name.c_str(), textX-1, textY-1, fontSize, BLACK);
        DrawText(name.c_str(), textX+1, textY-1, fontSize, BLACK);
        DrawText(name.c_str(), textX-1, textY+1, fontSize, BLACK);
        
        // Main Text
        DrawText(name.c_str(), textX, textY, fontSize, WHITE); // White text on Dark bg
    }
    */

    // Draw Exit Button
    bool hover = CheckCollisionPointRec(GetMousePosition(), exitBtnRect);
    DrawRectangleRec(exitBtnRect, hover ? Fade(RED, 0.8f) : Fade(RED, 0.6f));
    DrawRectangleLinesEx(exitBtnRect, 2, WHITE);
    DrawText("EXIT", exitBtnRect.x + 25, exitBtnRect.y + 10, 20, WHITE);
}
