#include "parking_lot.h"
#include <cmath>

ParkingLot::ParkingLot() : graph(5) {
    bg = LoadTexture("src/graphics/pklot1.png");

    // Initialize Vehicle Position at Start (Entrance)
    vehiclePos = {793, 771};

    // Initialize 4 Parking Slots
    float w = 60;
    float h = 100;

    // Slot 1: 83, 729 (Bottom Left)
    slots.push_back({{83 - w / 2, 729 - h / 2, w, h}, {83, 729}, false, false});

    // Slot 2: 325, 725 (Bottom Middle)
    slots.push_back({{325 - w / 2, 725 - h / 2, w, h}, {325, 725}, false, false});

    // Slot 3: 660, 62 (Top Right)
    slots.push_back({{660 - w / 2, 62 - h / 2, w, h}, {660, 62}, false, false});

    // Slot 4: 164, 60 (Top Left)
    slots.push_back({{164 - w / 2, 60 - h / 2, w, h}, {164, 60}, false, false});
}

ParkingLot::~ParkingLot() {
    if (bg.id != 0) UnloadTexture(bg);
}

void ParkingLot::update() {
    // Exit Button Logic
    if (CheckCollisionPointRec(GetMousePosition(), exitBtnRect)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            exitClicked = true;
        }
    }

    // Vehicle Movement Logic (Animation)
    if (isMoving) {
        Vector2 dir = {targetPos.x - vehiclePos.x, targetPos.y - vehiclePos.y};
        float dist = sqrt(dir.x * dir.x + dir.y * dir.y);

        if (dist < speed) {
            vehiclePos = targetPos;

            // Check if more waypoints exist
            if (!pathQueue.empty()) {
                targetPos = pathQueue.front();
                pathQueue.erase(pathQueue.begin());
            } else {
                isMoving = false;
                // Arrived at final destination (Parking Slot)
                parked = true;
                parkTimer = 0.0f;
            }
        } else {
            vehiclePos.x += dir.x / dist * speed;
            vehiclePos.y += dir.y / dist * speed;
        }
    }

    // Parking Timer Logic
    if (parked && !exitClicked) {
        parkTimer += GetFrameTime();
        if (parkTimer >= 5.0f) { // 7 initial...
            exitRequested = true;
        }
    }

    // Slot Interaction Logic (Only if not moving)
    if (!isMoving && !parked) { // Disable interaction if already parked
        for (size_t i = 0; i < slots.size(); i++) {
            slots[i].hover = CheckCollisionPointRec(GetMousePosition(), slots[i].rect);

            if (slots[i].hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                // Define Path based on slot index
                pathQueue.clear();

                if (i == 0) { // Slot 1: 83, 729 (Bottom Left)
                    pathQueue.push_back({793, 510}); // Up to lane
                    pathQueue.push_back({83, 510});  // Left to slot X
                    pathQueue.push_back({83, 650});  // Down into slot
                } else if (i == 1) { // Slot 2: 325, 725 (Bottom Mid)
                    pathQueue.push_back({793, 510}); // Up to lane
                    pathQueue.push_back({325, 510}); // Left to slot X
                    pathQueue.push_back({325, 650}); // Down into slot
                } else if (i == 2) { // Slot 3: 660, 62 (Top Right)
                    pathQueue.push_back({793, 300}); // Up to top lane (long way up)
                    pathQueue.push_back({660, 300}); // Left to slot X
                    pathQueue.push_back({660, 150});  // Up into slot
                } else if (i == 3) { // Slot 4: 164, 60 (Top Left)
                    pathQueue.push_back({793, 300}); // Up to top lane
                    pathQueue.push_back({164, 300}); // Left to slot X
                    pathQueue.push_back({164, 150});  // Up into slot
                }

                // Start moving to first point
                if (!pathQueue.empty()) {
                    targetPos = pathQueue.front();
                    pathQueue.erase(pathQueue.begin());
                    isMoving = true;
                }
            }
        }
    }
}

void ParkingLot::draw() {
    ClearBackground(DARKGRAY);

    if (bg.id != 0) DrawTexture(bg, 0, 0, WHITE);
    else DrawText("PARKING LOT SUB-ZONE (Image Not Found)", 300, 50, 30, WHITE);

    // Draw Exit Button
    bool hover = CheckCollisionPointRec(GetMousePosition(), exitBtnRect);
    DrawRectangleRec(exitBtnRect, hover ? Fade(RED, 0.8f) : Fade(RED, 0.6f));
    DrawRectangleLinesEx(exitBtnRect, 2, WHITE);
    DrawText("EXIT", exitBtnRect.x + 25, exitBtnRect.y + 10, 20, WHITE);
}
