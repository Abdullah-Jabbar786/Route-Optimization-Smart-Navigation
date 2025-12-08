#include "bank.h"
#include "mainZone.h"
#include <cmath>

Bank::Bank() : graph(5)
{
    bg = LoadTexture("src/graphics/bk4.png");
    graph.addNode("Entrance");
    graph.addNode("Tellers");
    graph.addNode("WaitingArea");
    graph.addNode("ATM");
    graph.addNode("Vault");
    graph.addNode("LoanOffice");

    graph.addEdge("Entrance", "ATM", 5);
    graph.addEdge("Entrance", "WaitingArea", 8);
    graph.addEdge("Tellers", "Vault", 8);
    graph.addEdge("Vault", "ATM", 8);
    // graph.addEdge("Tellers", "ATM", 9);
    graph.addEdge("Tellers", "LoanOffice", 4);
    graph.addEdge("WaitingArea", "LoanOffice", 3);
    graph.addEdge("Entrance", "Vault", 7);

    nodePos["Entrance"] = {205, 673};
    nodePos["ATM"] = {169, 503};
    nodePos["Vault"] = {171, 293};
    nodePos["Tellers"] = {514, 205};
    nodePos["WaitingArea"] = {675, 580};
    nodePos["LoanOffice"] = {675, 360};

    vehiclePos = nodePos["Entrance"];
}

Bank::~Bank()
{
    if (bg.id != 0)
        UnloadTexture(bg);
}

void Bank::update(MainZone &mz)
{
    // Exit Button Logic
    if (CheckCollisionPointRec(GetMousePosition(), exitBtnRect))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            exitClicked = true;
            
            showInfo = false;
            currentPath.clear();
        }
    }

    // Node Click Logic
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        for (auto &[name, pos] : nodePos)
        {
            if (CheckCollisionPointCircle(GetMousePosition(), pos, 20))
            {
                if (name != "Entrance")
                {
                    PathResult r = findShortestPath(graph, "Entrance", name);
                    if (r.distance != -1)
                    {
                        distance = r.distance;
                        walkingTime = (float)distance / 1.0f / 3.0f; // Mins
                        targetName = name;
                        currentPath = r.path;
                        showInfo = true;

                        // Push to Main Stack
                        mz.pushExternalHistory(name, (float)distance, walkingTime);

                        vehiclePos = nodePos[name];
                    }
                }
            }
        }
    }
}

void Bank::draw()
{
    ClearBackground(LIGHTGRAY); // Changed color for Bank
    DrawText("BANK SUB-ZONE", 350, 50, 30, DARKBLUE);

    if (bg.id != 0)
        DrawTexture(bg, 0, 0, WHITE);

    for (auto &[name, pos] : nodePos)
    {
        DrawCircleV(pos, 10, DARKGRAY); // Changed color for Bank

        // Text: Boldish (Shadow)
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

    if (!currentPath.empty())
    {
        for (size_t i = 0; i < currentPath.size() - 1; i++)
        {
            if (nodePos.count(currentPath[i]) && nodePos.count(currentPath[i + 1]))
            {
                DrawLineEx(nodePos[currentPath[i]], nodePos[currentPath[i + 1]], 6, GREEN);
            }
        }
    }

    // Draw Info Box
    if (showInfo)
    {
        int w = 220, h = 100;
        int x = GetScreenWidth() - w - 20, y = GetScreenHeight() - h - 20;
        DrawRectangle(x, y, w, h, Fade(SKYBLUE, 0.9f));
        DrawRectangleLines(x, y, w, h, DARKBLUE);

        DrawText("TARGET INFO", x + 10, y + 10, 24, DARKBLUE);
        DrawText(("To: " + targetName).c_str(), x + 10, y + 35, 18, BLACK); // Smaller font for name to fit
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
