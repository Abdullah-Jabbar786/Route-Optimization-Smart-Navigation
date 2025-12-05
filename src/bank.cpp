#include "bank.h"

Bank::Bank() : graph(5){
    graph.addNode("Entrance");
    graph.addNode("Tellers");
    graph.addNode("Vault");
    graph.addNode("ATM");
    
    graph.addEdge("Entrance", "ATM", 5);
    graph.addEdge("Entrance", "Tellers", 10);
    graph.addEdge("Tellers", "Vault", 8);

    nodePos["Entrance"] = {450, 750};
    nodePos["ATM"] = {250, 600};
    nodePos["Tellers"] = {450, 400};
    nodePos["Vault"] = {450, 150};

    vehiclePos = nodePos["Entrance"];
}

Bank::~Bank() {
    if(bg.id != 0) UnloadTexture(bg);
}

void Bank::update() {
    // Exit Button Logic
    if (CheckCollisionPointRec(GetMousePosition(), exitBtnRect))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            exitClicked = true;
        }
    }
}

void Bank::draw() {
    ClearBackground(LIGHTGRAY); // Changed color for Bank
    DrawText("BANK SUB-ZONE", 350, 50, 30, DARKBLUE);
    
    if(bg.id != 0) DrawTexture(bg, 0, 0, WHITE);

    for(auto &[name, pos] : nodePos) {
        DrawCircleV(pos, 10, DARKGREEN); // Changed color for Bank
        
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
