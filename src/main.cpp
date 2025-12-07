#include <raylib.h>
#include "mainZone.h"
#include "amusement_park.h"
#include "hotel.h"
#include "food_court.h"
#include "bank.h"
#include "parking_lot.h"
#include "building_report.h"
#include <cmath>

class Icon {
public:
    Texture2D tex;
    Vector2 pos;
    float baseSize;
    float scale;
    float rotation;
    bool selected;
};

bool MouseOverIcon(const Icon &icon) {
    float s = icon.baseSize * icon.scale;
    Rectangle r = {icon.pos.x - s / 2, icon.pos.y - s / 2, s, s};
    return CheckCollisionPointRec(GetMousePosition(), r);
}

enum AppScreen {
    LOADING_SCREEN,
    BUILDING_REPORT,
    MAIN_ZONE,
    AMUSEMENT_PARK,
    HOTEL,
    FOOD_COURT,
    BANK,
    PARKING_LOT,
    TELEPORT_TO_END,
    ENDING_SCREEN
};

int main() {
    // Initialize Building Report Data
    BuildingReport tree;

    tree.insert("Food & Dining", "PizzaParadise");
    tree.insert("Food & Dining", "WokWonders");
    tree.insert("Food & Dining", "SushiSpot");
    tree.insert("Food & Dining", "BurgerJoint");
    tree.insert("Food & Dining", "CoffeePastre");
    tree.insert("Food & Dining", "FusionBakery");
    tree.insert("Food & Dining", "FoodCourtandDrink");
    tree.insert("Food & Dining", "FoodCourt");

    tree.insert("Amusement Park", "RollerCoaster");
    tree.insert("Amusement Park", "FerrisWheel");
    tree.insert("Amusement Park", "ObservationTower");
    tree.insert("Amusement Park", "WaterSlide");
    tree.insert("Amusement Park", "cupride");
    tree.insert("Amusement Park", "DropTower");
    tree.insert("Amusement Park", "TrainStationRide");

    tree.insert("Hotel Services", "PoolandSpaArea");
    tree.insert("Hotel Services", "GuestRoomsWingA");
    tree.insert("Hotel Services", "MainLobbyandReception");
    tree.insert("Hotel Services", "MarketPlaceLane");
    tree.insert("Hotel Services", "EntertainmentZone");
    tree.insert("Hotel Services", "ElectronicsHub");
    tree.insert("Hotel Services", "ConferenceCenter");

    tree.insert("Bank Services", "Tellers");
    tree.insert("Bank Services", "Vault");
    tree.insert("Bank Services", "ATM");

    tree.insert("Sports", "FootballGround");

    tree.insert("Shops", "FusionBakery");
    tree.insert("Shops", "MarketPlaceLane");
    tree.insert("Shops", "ElectronicsHub");

    InitWindow(900, 850, "Route Optimization and Smart Navigation");
    SetTargetFPS(60);
    AppScreen currentScreen = LOADING_SCREEN;
    int transitionTimer = 0;

    // Load Resources
    Texture2D bg = LoadTexture("src/graphics/lgbg2.jpg");
    Texture2D logo = LoadTexture("src/graphics/startlogo.png");
    Texture2D slogo = LoadTexture("src/graphics/select.png");
    Texture2D hat = LoadTexture("src/graphics/hat.png");
    Texture2D cart = LoadTexture("src/graphics/cart.png");
    Texture2D car = LoadTexture("src/graphics/car.png");
    Texture2D exitLogo = LoadTexture("src/graphics/exitlogo.png");

    Texture2D selectedCharacter;
    bool characterChosen = false;

    Icon icons[3] = {
        {hat, {250, 550}, 130, 1.0f, 0, false},
        {cart, {450, 550}, 130, 1.0f, 0, false},
        {car, {650, 550}, 130, 1.0f, 0, false}
    };

    MainZone mainZone;
    AmusementPark amusementPark;
    Hotel hotel;
    FoodCourt foodCourt;
    Bank bank;
    ParkingLot parkingLot;

    Texture2D mainMap = LoadTexture("src/graphics/mainmap.jpg");

    // Animation States
    bool teleporting = false;
    float teleportScale = 1.0f;
    int teleportTimer = 0;
    int endTeleportTimer = 0;

    char reportSearchText[64] = "";
    int reportSearchCount = 0;

    // Cache for ending screen history
    vector<TravelState> finalHistory;

    while (!WindowShouldClose()) {
        if (currentScreen == LOADING_SCREEN) {
            Rectangle reportBtn = {350, 750, 200, 40};
            if (CheckCollisionPointRec(GetMousePosition(), reportBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !teleporting) {
                currentScreen = BUILDING_REPORT;
            }
            if (!teleporting) {
                for (int i = 0; i < 3; i++) {
                    icons[i].rotation += 1.0f;
                    icons[i].scale = MouseOverIcon(icons[i]) ? 1.35f : 1.0f;
                    if (MouseOverIcon(icons[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        for (int j = 0; j < 3; j++)
                            icons[j].selected = false;
                        icons[i].selected = true;
                    }
                    if (icons[i].selected) {
                        selectedCharacter = icons[i].tex;
                        characterChosen = true;
                        teleporting = true;
                        teleportTimer = 0;
                        teleportScale = icons[i].scale;
                    }
                }
            } else {
                teleportTimer++;
                teleportScale -= 0.02f;
                icons[0].rotation += 15.0f;
                icons[1].rotation += 15.0f;
                icons[2].rotation += 15.0f;

                if (teleportTimer > 60) {
                    currentScreen = MAIN_ZONE;
                    teleporting = false;
                    transitionTimer = 60; 
                }
            }
        } else if (currentScreen == BUILDING_REPORT) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Rectangle backBtn = {20, 20, 100, 40};
                if (CheckCollisionPointRec(GetMousePosition(), backBtn)) {
                    currentScreen = LOADING_SCREEN;
                    reportSearchCount = 0;
                    reportSearchText[0] = '\0';
                }
            }

            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (reportSearchCount < 63)) {
                    reportSearchText[reportSearchCount] = (char)key;
                    reportSearchText[reportSearchCount + 1] = '\0';
                    reportSearchCount++;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (reportSearchCount > 0) {
                    reportSearchCount--;
                    reportSearchText[reportSearchCount] = '\0';
                }
            }
        } else if (currentScreen == MAIN_ZONE) {
            mainZone.update();
            if (mainZone.isLoadingComplete()) {
                string sub = mainZone.getEnteredSubMap();
                if (sub == "AmusementPark") currentScreen = AMUSEMENT_PARK;
                else if (sub == "Hotel") currentScreen = HOTEL;
                else if (sub == "FoodCourt") currentScreen = FOOD_COURT;
                else if (sub == "Bank") currentScreen = BANK;
                else if (sub == "ParkingLot") currentScreen = PARKING_LOT;
            }
        } else if (currentScreen == AMUSEMENT_PARK) {
            amusementPark.update(mainZone);
            if (amusementPark.isExitClicked()) {
                amusementPark.resetExit();
                mainZone.resetLoading();
                currentScreen = MAIN_ZONE;
            }
        } else if (currentScreen == HOTEL) {
            hotel.update(mainZone);
            if (hotel.isExitClicked()) {
                hotel.resetExit();
                mainZone.resetLoading();
                currentScreen = MAIN_ZONE;
            }
        } else if (currentScreen == FOOD_COURT) {
            foodCourt.update(mainZone);
            if (foodCourt.isExitClicked()) {
                foodCourt.resetExit();
                mainZone.resetLoading();
                currentScreen = MAIN_ZONE;
            }
        } else if (currentScreen == BANK) {
            bank.update(mainZone);
            if (bank.isExitClicked()) {
                bank.resetExit();
                mainZone.resetLoading();
                currentScreen = MAIN_ZONE;
            }
        } else if (currentScreen == PARKING_LOT) {
            parkingLot.update();
            if (parkingLot.isExitClicked()) {
                parkingLot.resetExit();
                mainZone.resetLoading();
                currentScreen = MAIN_ZONE;
            }
            if (parkingLot.isExitRequested()) {
                currentScreen = TELEPORT_TO_END;
                endTeleportTimer = 0;
            }
        } else if (currentScreen == TELEPORT_TO_END) {
            endTeleportTimer++;
            icons[0].rotation += 15.0f;
            icons[1].rotation += 15.0f;
            icons[2].rotation += 15.0f;

            if (endTeleportTimer > 180) { // 3 Seconds
                currentScreen = ENDING_SCREEN;
                finalHistory = mainZone.getTravelHistory(); // Fetch ONCE optimization
            }
        }

        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        if (currentScreen == LOADING_SCREEN) {
            DrawTexture(bg, 0, 0, WHITE);
            DrawTexture(logo, 60, 200, WHITE);
            DrawTexture(slogo, 250, 350, WHITE);

            for (int i = 0; i < 3; i++) {
                if (teleporting && !icons[i].selected) continue;

                float s = icons[i].baseSize * (teleporting ? teleportScale : icons[i].scale);
                Rectangle src = {0, 0, (float)icons[i].tex.width, (float)icons[i].tex.height};
                Rectangle dest = {icons[i].pos.x, icons[i].pos.y, s, s};
                Vector2 origin = {s / 2, s / 2};

                if (icons[i].selected && !teleporting)
                    DrawCircleLines(icons[i].pos.x, icons[i].pos.y, s / 2 + 10, RED);

                Color tint = WHITE;
                if (teleporting) tint = Fade(WHITE, teleportScale);

                DrawTexturePro(icons[i].tex, src, dest, origin, icons[i].rotation, tint);
            }

            if (teleporting) {
                DrawText("Teleporting...", 350, 700, 30, Fade(GREEN, 0.8f + 0.2f * sin(GetTime() * 10)));
            }

            Rectangle reportBtn = {350, 750, 200, 40};
            bool hover = CheckCollisionPointRec(GetMousePosition(), reportBtn);
            DrawRectangleRec(reportBtn, hover ? SKYBLUE : LIGHTGRAY);
            DrawRectangleLinesEx(reportBtn, 2, DARKBLUE);
            DrawText("Building Report", reportBtn.x + 20, reportBtn.y + 10, 20, DARKBLUE);
        } else if (currentScreen == BUILDING_REPORT) {
            DrawTexture(bg, 0, 0, WHITE);

            Rectangle backBtn = {20, 20, 100, 40};
            bool backHover = CheckCollisionPointRec(GetMousePosition(), backBtn);

            DrawRectangleRec(backBtn, backHover ? Fade(RED, 0.8f) : Fade(RED, 0.6f));
            DrawRectangleLinesEx(backBtn, 2, WHITE);
            DrawText("BACK", backBtn.x + 25, backBtn.y + 10, 20, WHITE);

            DrawText("Search Building:", 200, 30, 20, BLACK);

            Rectangle searchBox = {380, 25, 220, 30};
            DrawRectangleRec(searchBox, Fade(LIGHTGRAY, 0.9f));
            DrawRectangleLinesEx(searchBox, 1, DARKGRAY);
            DrawText(reportSearchText, searchBox.x + 5, searchBox.y + 5, 20, BLACK);

            if (reportSearchCount > 0) {
                string cat = tree.findCategoryOfBuilding(reportSearchText);
                if (!cat.empty()) {
                    DrawText(("Found in: " + cat).c_str(), 380, 60, 20, GREEN);
                } else {
                    DrawText("Not Found", 380, 60, 20, RED);
                }
            }

            tree.draw(50, 150);
        } else if (currentScreen == MAIN_ZONE) {
            if (transitionTimer > 0) {
                transitionTimer--;
                DrawTexture(mainMap, 0, 0, Fade(WHITE, 1.0f - (float)transitionTimer / 60.0f));
                DrawRectangle(0, 0, 900, 850, Fade(BLACK, (float)transitionTimer / 60.0f));
                DrawText("Entering Main Zone...", 300, 400, 30, Fade(WHITE, (float)transitionTimer / 60.0f));
            } else {
                DrawTexture(mainMap, 0, 0, WHITE);
                mainZone.draw();
                if (characterChosen) {
                    Vector2 vPos = mainZone.getVehiclePos();
                    float s = 50.0f;
                    Rectangle src = {0, 0, (float)selectedCharacter.width, (float)selectedCharacter.height};
                    Rectangle dest = {vPos.x, vPos.y, s, s};
                    Vector2 origin = {s / 2, s / 2};
                    DrawTexturePro(selectedCharacter, src, dest, origin, 0.0f, WHITE);
                }
            }
        } else if (currentScreen == AMUSEMENT_PARK) {
            amusementPark.draw();
            if (characterChosen) {
                Vector2 vPos = amusementPark.getVehiclePos();
                float s = 60.0f;
                Rectangle src = {0, 0, (float)selectedCharacter.width, (float)selectedCharacter.height};
                Rectangle dest = {vPos.x, vPos.y, s, s};
                Vector2 origin = {s / 2, s / 2};
                DrawTexturePro(selectedCharacter, src, dest, origin, 0.0f, WHITE);
            }
        } else if (currentScreen == HOTEL) {
            hotel.draw();
            if (characterChosen) {
                Vector2 vPos = hotel.getVehiclePos();
                float s = 60.0f;
                Rectangle src = {0, 0, (float)selectedCharacter.width, (float)selectedCharacter.height};
                Rectangle dest = {vPos.x, vPos.y, s, s};
                Vector2 origin = {s / 2, s / 2};
                DrawTexturePro(selectedCharacter, src, dest, origin, 0.0f, WHITE);
            }
        } else if (currentScreen == FOOD_COURT) {
            foodCourt.draw();
            if (characterChosen) {
                Vector2 vPos = foodCourt.getVehiclePos();
                float s = 60.0f;
                Rectangle src = {0, 0, (float)selectedCharacter.width, (float)selectedCharacter.height};
                Rectangle dest = {vPos.x, vPos.y, s, s};
                Vector2 origin = {s / 2, s / 2};
                DrawTexturePro(selectedCharacter, src, dest, origin, 0.0f, WHITE);
            }
        } else if (currentScreen == BANK) {
            bank.draw();
            if (characterChosen) {
                Vector2 vPos = bank.getVehiclePos();
                float s = 60.0f;
                Rectangle src = {0, 0, (float)selectedCharacter.width, (float)selectedCharacter.height};
                Rectangle dest = {vPos.x, vPos.y, s, s};
                Vector2 origin = {s / 2, s / 2};
                DrawTexturePro(selectedCharacter, src, dest, origin, 0.0f, WHITE);
            }
        } else if (currentScreen == PARKING_LOT) {
            parkingLot.draw();
            if (characterChosen) {
                Vector2 vPos = parkingLot.getVehiclePos();
                float s = 60.0f;
                Rectangle src = {0, 0, (float)selectedCharacter.width, (float)selectedCharacter.height};
                Rectangle dest = {vPos.x, vPos.y, s, s};
                Vector2 origin = {s / 2, s / 2};
                DrawTexturePro(selectedCharacter, src, dest, origin, 0.0f, WHITE);
            }
        } else if (currentScreen == TELEPORT_TO_END) {
            DrawTexture(mainMap, 0, 0, Fade(WHITE, 0.2f));
            DrawRectangle(0, 0, 900, 850, Fade(BLACK, 0.8f));

            int boxW = 500;
            int boxH = 300;
            int boxX = (GetScreenWidth() - boxW) / 2;
            int boxY = (GetScreenHeight() - boxH) / 2;

            // Transition Box
            DrawRectangle(boxX, boxY, boxW, boxH, Fade(Color{20, 20, 30, 255}, 0.95f));
            DrawRectangleLinesEx({(float)boxX, (float)boxY, (float)boxW, (float)boxH}, 2, GOLD);

            DrawText("Moving To Ending Screen", boxX + 40, boxY + 40, 30, WHITE);

            string dots = "";
            int dotStage = (endTeleportTimer / 15) % 4;
            if (dotStage == 1) dots = ".";
            else if (dotStage == 2) dots = "..";
            else if (dotStage == 3) dots = "...";

            DrawText("Status:", boxX + 40, boxY + 100, 20, LIGHTGRAY);
            DrawText(("Exiting" + dots).c_str(), boxX + 120, boxY + 100, 20, GOLD);

            float progress = (float)endTeleportTimer / 180.0f;
            if (progress > 1.0f) progress = 1.0f;

            int barW = 420;
            int barH = 10;
            int barX = boxX + 40;
            int barY = boxY + 220;

            DrawRectangle(barX, barY, barW, barH, DARKGRAY);
            DrawRectangle(barX, barY, (int)(barW * progress), barH, GREEN);

            if (characterChosen) {
                float s = 80.0f;
                DrawTexturePro(selectedCharacter, {0, 0, (float)selectedCharacter.width, (float)selectedCharacter.height},
                               {(float)boxX - 40, (float)boxY + 100, s, s}, {0, 0}, 0.0f, WHITE);
            }
        } else if (currentScreen == ENDING_SCREEN) {
            DrawTexture(bg, 0, 0, WHITE);

            float s = 1.0f;
            int lw = exitLogo.width * s;
            int lx = (GetScreenWidth() - lw) / 2;
            int ly = 40;

            DrawTextureEx(exitLogo, {(float)lx, (float)ly}, 0.0f, s, WHITE);

            // Draw Travel History
            int hy = ly + (exitLogo.height * s) + 40;

            const char* hTitle = "Travel History:";
            DrawText(hTitle, 102, hy + 2, 30, LIGHTGRAY);
            DrawText(hTitle, 100, hy, 30, DARKBLUE);
            hy += 40;

            int startHy = hy;
            int colOffset = 0;

            for (size_t i = 0; i < finalHistory.size(); i++) {
                if (hy > GetScreenHeight() - 150) { 
                    colOffset += 350; 
                    hy = startHy;
                }

                int indent = 100 + (finalHistory[i].depth * 30) + colOffset;

                DrawCircle(indent - 10, hy + 10, 3, DARKBLUE);
                DrawText(finalHistory[i].placeName.c_str(), indent, hy, 20, DARKGRAY);

                hy += 25;
            }

            if (!finalHistory.empty()) {
                TravelState finalState = finalHistory.back();
                hy += 20;

                string distStr = "Total Distance: " + to_string((int)finalState.cumulativeDistance) + " units";
                DrawText(distStr.c_str(), 100 + colOffset, hy, 22, DARKBLUE);

                hy += 30;

                int totalMins = (int)finalState.cumulativeTime;
                string timeStr = "Total Time: ";
                if (totalMins >= 60) {
                    int hrs = totalMins / 60;
                    int mins = totalMins % 60;
                    timeStr += to_string(hrs) + " hr " + to_string(mins) + " mins";
                } else {
                    timeStr += to_string(totalMins) + " mins";
                }

                DrawText(timeStr.c_str(), 100 + colOffset, hy, 22, DARKBLUE);
            }

            if ((int)(GetTime() * 2) % 2 == 0) { 
                DrawText("Press any button to exit", 300, 750, 24, Fade(DARKGRAY, 0.8f));
            }

            if (GetKeyPressed() != 0) {
                break;
            }

            const char* footer = "(c) 2025 AHM. All rights reserved.";
            int fSize = 20;
            int fW = MeasureText(footer, fSize);
            DrawText(footer, GetScreenWidth() - fW - 20, GetScreenHeight() - 40, fSize, DARKGRAY);
        }

        EndDrawing();
    }

    UnloadTexture(bg);
    UnloadTexture(logo);
    UnloadTexture(slogo);
    UnloadTexture(exitLogo);
    UnloadTexture(hat);
    UnloadTexture(cart);
    UnloadTexture(car);
}
