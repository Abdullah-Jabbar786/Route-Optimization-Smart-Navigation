#include <raylib.h>
#include "mainZone.h"
#include "amusement_park.h"
#include "hotel.h"
#include "food_court.h"
#include "bank.h"
#include "parking_lot.h"
#include <cmath>

class Icon{
public:
    Texture2D tex;
    Vector2 pos;
    float baseSize;
    float scale;
    float rotation;
    bool selected;
};

bool MouseOverIcon(const Icon &icon)
{
    float s = icon.baseSize * icon.scale;
    Rectangle r = {icon.pos.x - s / 2, icon.pos.y - s / 2, s, s};
    return CheckCollisionPointRec(GetMousePosition(), r);
}

enum AppScreen
{
    LOADING_SCREEN,
    MAIN_ZONE,
    AMUSEMENT_PARK,
    HOTEL,
    FOOD_COURT,
    BANK,
    PARKING_LOT
};

int main(){
    InitWindow(900, 850, "Smart Navigation and Route Optimization");
    SetTargetFPS(60);
    AppScreen currentScreen = LOADING_SCREEN;
    int transitionTimer = 0;

    Texture2D bg = LoadTexture("src/graphics/lgbg2.jpg");
    Texture2D logo = LoadTexture("src/graphics/startlogo.png");
    Texture2D slogo = LoadTexture("src/graphics/select.png");
    Texture2D hat = LoadTexture("src/graphics/hat.png");
    Texture2D cart = LoadTexture("src/graphics/cart.png");
    Texture2D car = LoadTexture("src/graphics/car.png");

    Texture2D selectedCharacter;
    bool characterChosen = false;

    Icon icons[3] = {{hat, {250, 550}, 130, 1.0f, 0, false}, {cart, {450, 550}, 130, 1.0f, 0, false}, {car, {650, 550}, 130, 1.0f, 0, false}};
    MainZone mainZone;
    AmusementPark amusementPark;
    Hotel hotel;
    FoodCourt foodCourt;
    Bank bank;
    ParkingLot parkingLot;

    Texture2D mainMap = LoadTexture("src/graphics/mainmap.jpg");
    Vector2 entrancePos = {229, 764};

    // Teleport animation state
    bool teleporting = false;
    float teleportScale = 1.0f;
    int teleportTimer = 0;

    while(!WindowShouldClose()){
        if(currentScreen == LOADING_SCREEN){
            if(!teleporting){
                for (int i = 0; i < 3; i++){
                    icons[i].rotation += 1.0f;
                    icons[i].scale = MouseOverIcon(icons[i]) ? 1.35f : 1.0f;
                    if (MouseOverIcon(icons[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                        for (int j = 0; j < 3; j++)
                            icons[j].selected = false;
                        icons[i].selected = true;
                    }
                    if(icons[i].selected){
                        selectedCharacter = icons[i].tex;
                        characterChosen = true;
                        teleporting = true;
                        teleportTimer = 0;
                        teleportScale = icons[i].scale;
                    }
                }
            }
            else
            {
                // Teleport Animation Logic
                teleportTimer++;
                teleportScale -= 0.02f;
                icons[0].rotation += 15.0f; // Spin faster
                icons[1].rotation += 15.0f;
                icons[2].rotation += 15.0f;

                if(teleportTimer > 60){
                    currentScreen = MAIN_ZONE;
                    teleporting = false;
                    transitionTimer = 60; // For MainZone fade-in
                }
            }
        }
        else if (currentScreen == MAIN_ZONE){
            mainZone.update();
            if(mainZone.isLoadingComplete()){
                string sub = mainZone.getEnteredSubMap();
                if (sub == "AmusementPark")
                    currentScreen = AMUSEMENT_PARK;
                else if (sub == "Hotel")
                    currentScreen = HOTEL;
                else if (sub == "FoodCourt")
                    currentScreen = FOOD_COURT;
                else if (sub == "Bank")
                    currentScreen = BANK;
                else if (sub == "ParkingLot")
                    currentScreen = PARKING_LOT;
            }
        }
        else if (currentScreen == AMUSEMENT_PARK)
        {
            amusementPark.update();
            if (amusementPark.isExitClicked()) {
                amusementPark.resetExit();
                mainZone.resetLoading();
                currentScreen = MAIN_ZONE;
            }
        }
        else if (currentScreen == HOTEL)
        {
            hotel.update();
            if (hotel.isExitClicked())
            {
                hotel.resetExit();
                mainZone.resetLoading();
                currentScreen = MAIN_ZONE;
            }
        }
        else if (currentScreen == FOOD_COURT)
        {
            foodCourt.update();
            if (foodCourt.isExitClicked())
            {
                foodCourt.resetExit();
                mainZone.resetLoading();
                currentScreen = MAIN_ZONE;
            }
        }
        else if (currentScreen == BANK)
        {
            bank.update();
            if (bank.isExitClicked())
            {
                bank.resetExit();
                mainZone.resetLoading();
                currentScreen = MAIN_ZONE;
            }
        }
        else if (currentScreen == PARKING_LOT)
        {
            parkingLot.update();
            if (parkingLot.isExitClicked())
            {
                parkingLot.resetExit();
                mainZone.resetLoading();
                currentScreen = MAIN_ZONE;
            }
        }

        BeginDrawing();

        ClearBackground(LIGHTGRAY);
        if (currentScreen == LOADING_SCREEN)
        {
            DrawTexture(bg, 0, 0, WHITE);
            DrawTexture(logo, 60, 200, WHITE);
            DrawTexture(slogo, 250, 350, WHITE);

            for (int i = 0; i < 3; i++)
            {
                if (teleporting && !icons[i].selected)
                    continue; // Hide others during teleport

                float s = icons[i].baseSize * (teleporting ? teleportScale : icons[i].scale);
                Rectangle src = {0, 0, (float)icons[i].tex.width, (float)icons[i].tex.height};
                Rectangle dest = {icons[i].pos.x, icons[i].pos.y, s, s};
                Vector2 origin = {s / 2, s / 2};

                if (icons[i].selected && !teleporting)
                    DrawCircleLines(icons[i].pos.x, icons[i].pos.y, s / 2 + 10, RED);

                Color tint = WHITE;
                if (teleporting)
                    tint = Fade(WHITE, teleportScale); // Fade out

                DrawTexturePro(icons[i].tex, src, dest, origin, icons[i].rotation, tint);
            }

            if (teleporting)
            {
                DrawText("Teleporting...", 350, 700, 30, Fade(GOLD, 0.8f + 0.2f * sin(GetTime() * 10)));
            }
        }
        else if (currentScreen == MAIN_ZONE)
        {
            if (transitionTimer > 0)
            {
                transitionTimer--;
                // Aesthetic transition
                DrawTexture(mainMap, 0, 0, Fade(WHITE, 1.0f - (float)transitionTimer / 60.0f));
                DrawRectangle(0, 0, 900, 850, Fade(BLACK, (float)transitionTimer / 60.0f));
                DrawText("Entering Main Zone...", 300, 400, 30, Fade(WHITE, (float)transitionTimer / 60.0f));
            }
            else{
                DrawTexture(mainMap, 0, 0, WHITE);
                mainZone.draw();
                if (characterChosen)
                    DrawTexture(selectedCharacter, entrancePos.x, entrancePos.y, WHITE);
            }
        }
        else if (currentScreen == AMUSEMENT_PARK)
            amusementPark.draw();
        else if (currentScreen == HOTEL)
            hotel.draw();
        else if (currentScreen == FOOD_COURT)
            foodCourt.draw();
        else if (currentScreen == BANK)
            bank.draw();
        else if (currentScreen == PARKING_LOT)
            parkingLot.draw();

        EndDrawing();
    }

    UnloadTexture(bg);
    UnloadTexture(logo);
    UnloadTexture(slogo);
}
