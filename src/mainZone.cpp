#include "mainZone.h"
#include <iostream>
#include <cmath>

PathResult MainZone::getShortestPath(const string& start, const string& end){
    if(useAStar) return findShortestPathAStar(graph, start, end, nodePos);
    return findShortestPath(graph,start,end);
}

string MainZone::getNodeAtPosition(Vector2 pos){
    for(auto &[name,p] : nodePos){
        if(CheckCollisionPointCircle(pos,p,30)) return name; // Increased radius to 30
    }
    return "";
}

MainZone::MainZone() : graph(10){
    graph.addNode("Entrance"); graph.addNode("Hotel"); graph.addNode("FoodCourt");
    graph.addNode("Bank"); graph.addNode("AmusementPark"); graph.addNode("ParkingLot");

    graph.addEdge("Entrance","Hotel",5);
    graph.addEdge("Entrance","FoodCourt",7);
    graph.addEdge("Entrance","Bank",10);
    graph.addEdge("Entrance","AmusementPark",12);
    graph.addEdge("Entrance","ParkingLot",6);
    graph.addEdge("Hotel","FoodCourt",4);
    graph.addEdge("Hotel","Bank",8);
    graph.addEdge("Hotel","AmusementPark",11);
    graph.addEdge("Hotel","ParkingLot",9);
    graph.addEdge("FoodCourt","Bank",3);
    graph.addEdge("FoodCourt","AmusementPark",7);
    graph.addEdge("FoodCourt","ParkingLot",5);
    graph.addEdge("Bank","AmusementPark",6);
    graph.addEdge("Bank","ParkingLot",4);
    graph.addEdge("AmusementPark","ParkingLot",5);

    nodePos["Entrance"]={229,764};
    nodePos["Hotel"]={138,431};
    nodePos["FoodCourt"]={633,230};
    nodePos["Bank"]={493,151};
    nodePos["AmusementPark"]={809,54};
    nodePos["ParkingLot"]={592,530};

    if(nodePos.count(startNode)) vehiclePos = nodePos[startNode];
}

void MainZone::updateSearchBar(){
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        searchBar.active = CheckCollisionPointRec(GetMousePosition(),searchBar.box);
    }

    if(searchBar.active){
        int key = GetCharPressed();
        while(key>0){
            if(key>=32 && key<=125 && searchBar.letterCount<49){
                searchBar.text[searchBar.letterCount++]=(char)key;
                searchBar.text[searchBar.letterCount]='\0';
            }
            key = GetCharPressed();
        }
        if(IsKeyPressed(KEY_BACKSPACE) && searchBar.letterCount>0)
            searchBar.text[--searchBar.letterCount]='\0';
        if(IsKeyPressed(KEY_ENTER) && searchBar.letterCount>0 && !vehicleMoving){
            string input = searchBar.text;
            string target = "";
            
            // Normalize input: remove spaces, handle case
            // But since our map keys are CamelCase (e.g. AmusementPark), we try to match loosely
            // Strategy: Check if input (minus spaces) matches any key (case-insensitive)
            
            string cleanInput = "";
            for(char c : input) if(c != ' ') cleanInput += tolower(c);
            
            for(auto &[name, pos] : nodePos){
                string cleanName = "";
                for(char c : name) cleanName += tolower(c);
                
                if(cleanInput == cleanName){
                    target = name;
                    break;
                }
            }

            if(!target.empty()){
                searchBar.searchTriggered=true;
                startNavigation(target);
            }
        }
    }
}

void MainZone::startNavigation(string target){
    PathResult r = getShortestPath(startNode,target);
    if(r.distance!=-1 && r.path.size()>1){
        currentPath=r.path;
        travelDistance=r.distance;
        targetNode=target;
        pathIndex=currentPath.size()-1; // Jump to end
        timeTaken=(float)travelDistance/60.0f*60.0f; // Calculate time
        
        // Teleport Logic
        vehicleMoving=false;
        vehiclePos = nodePos[target]; // Instant move
        
        // Trigger Delay before Loading
        waitingForTeleport = true;
        teleportDelayTimer = 0;
        enteringSubmap=targetNode;
        
        searchBar.searchTriggered=false;
        searchBar.active=false;
    } else vehicleMoving=false;
}

void MainZone::updateVehicleMovement(){
    if(!vehicleMoving) return;
    Vector2 dir={targetPos.x-vehiclePos.x,targetPos.y-vehiclePos.y};
    float d=sqrt(dir.x*dir.x+dir.y*dir.y);
    if(d>vehicleSpeedUnitsPerFrame){
        vehiclePos.x+=dir.x/d*vehicleSpeedUnitsPerFrame;
        vehiclePos.y+=dir.y/d*vehicleSpeedUnitsPerFrame;
    } else{
        vehiclePos=targetPos;
        pathIndex++;
        if((size_t)pathIndex<currentPath.size())
            targetPos=nodePos[currentPath[pathIndex]];
        else{
            vehicleMoving=false;
            // Arrival via travel
            waitingForTeleport = true;
            teleportDelayTimer = 0;
            enteringSubmap=targetNode;
            searchBar.searchTriggered=false;
        }
    }
}

void MainZone::update(){
    if(waitingForTeleport){
        teleportDelayTimer++;
        if(teleportDelayTimer > 60){ // Wait 1 second (60 frames)
            waitingForTeleport = false;
            showLoading = true;
            loadingTimer = 0;
            postLoadingTimer = 0;
            loadingComplete = false;
        }
        return;
    }

    if(showLoading) loadingTimer++; // Continue incrementing for animation
    
    if(!showLoading){
        updateSearchBar();
        updateVehicleMovement();

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !vehicleMoving){
            string clickedNode = getNodeAtPosition(GetMousePosition());
            if(!clickedNode.empty() && clickedNode != startNode){
                searchBar.searchTriggered = true;
                startNavigation(clickedNode);
            }
        }
        
        if(IsKeyPressed(KEY_A)) useAStar = true;
        if(IsKeyPressed(KEY_D)) useAStar = false;
    }
}

void MainZone::resetLoading(){
    loadingComplete = false;
    enteringSubmap = "";
    showLoading = false;
    loadingTimer = 0;
    postLoadingTimer = 0;
    searchBar.searchTriggered = false;
}

void MainZone::drawSearchBar(){
    Color border=searchBar.active?BLUE:GRAY;
    Color box=searchBar.active?LIGHTGRAY:WHITE;
    DrawRectangleRec(searchBar.box,box);
    DrawRectangleLinesEx(searchBar.box,2,border);
    DrawText(searchBar.text,(int)searchBar.box.x+5,(int)searchBar.box.y+10,20,BLACK);
    if(searchBar.active && (loadingTimer/30)%2==0) 
        DrawText("|",(int)searchBar.box.x+8+MeasureText(searchBar.text,20),(int)searchBar.box.y+10,20,BLACK);
}

void MainZone::drawNavigationInfo(){
    if(currentPath.empty() && !vehicleMoving) return;
    
    // Reduced size and moved to bottom right
    int w=260, h=140; 
    int m=20;
    int screenW=GetScreenWidth();
    int screenH=GetScreenHeight();
    
    Rectangle info={(float)screenW-w-m, (float)screenH-h-m, (float)w, (float)h};
    
    DrawRectangleRec(info, Fade(Color{230,230,230,255},0.95f));
    DrawRectangleLinesEx(info,2,DARKBLUE);
    
    int x=info.x+10, y=info.y+10, ls=25; // Reduced line spacing
    
    DrawText("CURRENT NAVIGATION",x,y,18,DARKBLUE); y+=ls;
    DrawText(("Target: "+targetNode).c_str(),x,y,16,BLACK); y+=ls;
    DrawText(("Distance: "+to_string(travelDistance)+" units").c_str(),x,y,16,BLACK); y+=ls;
    
    string t=(timeTaken<1.0f?"Time: <1 min":"Time: "+to_string((int)round(timeTaken))+" mins");
    DrawText(t.c_str(),x,y,16,BLACK);
    
    string s=vehicleMoving?"Status: Traveling...":"Status: Destination Set";
    DrawText(s.c_str(),x,info.y+h-25,16,vehicleMoving?GOLD:DARKGREEN);
    
    // Compact algorithm display
    string algo = useAStar ? "Algo: A*" : "Algo: Dijkstra";
    DrawText(algo.c_str(), x + 130, y, 16, PURPLE);
}

void MainZone::drawLoadingScreen(){
    // Fade in background
    if(loadingTimer < 30) loadingAlpha = (float)loadingTimer/30.0f;
    else if(loadingTimer > loadingDuration - 30 && postLoadingTimer > 30) loadingAlpha = 0.0f; // Fade out at very end
    else loadingAlpha = 1.0f;

    DrawRectangle(0,0,GetScreenWidth(),GetScreenHeight(),Fade(Color{10, 10, 20, 255}, 0.95f * loadingAlpha));
    
    int w=600,h=350;
    Rectangle lb={(float)(GetScreenWidth()-w)/2,(float)(GetScreenHeight()-h)/2,(float)w,(float)h};
    
    // Card Background
    DrawRectangleRec(lb, Fade(Color{30, 30, 40, 255}, loadingAlpha));
    DrawRectangleLinesEx(lb, 2, Fade(GOLD, loadingAlpha));
    
    // Animated Dots
    dotTimer++;
    string dots = "";
    if((dotTimer/20)%4 == 1) dots = ".";
    else if((dotTimer/20)%4 == 2) dots = "..";
    else if((dotTimer/20)%4 == 3) dots = "...";

    // Text Info
    int x = lb.x + 50;
    int y = lb.y + 60;
    
    DrawText(("Traveling to: " + enteringSubmap).c_str(), x, y, 30, Fade(WHITE, loadingAlpha));
    y += 60;
    
    DrawText("Status:", x, y, 20, Fade(GRAY, loadingAlpha));
    DrawText(("Teleporting" + dots).c_str(), x + 80, y, 20, Fade(GOLD, loadingAlpha));
    y += 40;
    
    DrawText("Distance:", x, y, 20, Fade(GRAY, loadingAlpha));
    DrawText((to_string(travelDistance) + " units").c_str(), x + 100, y, 20, Fade(SKYBLUE, loadingAlpha));
    y += 40;
    
    DrawText("Est. Time:", x, y, 20, Fade(GRAY, loadingAlpha));
    string tStr = (timeTaken<1.0f ? "<1 min" : to_string((int)round(timeTaken)) + " mins");
    DrawText(tStr.c_str(), x + 110, y, 20, Fade(SKYBLUE, loadingAlpha));
    
    // Progress Bar
    y += 80;
    float targetWidth = (lb.width - 100) * ((float)loadingTimer / loadingDuration);
    if(targetWidth > lb.width - 100) targetWidth = lb.width - 100;
    
    loadingBarWidth += (targetWidth - loadingBarWidth) * 0.1f; // Smooth lerp
    
    DrawRectangle(x, y, lb.width - 100, 10, Fade(DARKGRAY, loadingAlpha));
    DrawRectangle(x, y, (int)loadingBarWidth, 10, Fade(LIME, loadingAlpha));
    
    // Completion Logic with Post-Delay
    if(loadingTimer >= loadingDuration){
        postLoadingTimer++;
        if(postLoadingTimer > 60){ // Wait 1 more second at 100%
            showLoading=false;
            loadingComplete=true; 
            startNode=enteringSubmap;
            searchBar.letterCount=0;
            searchBar.text[0]='\0';
        }
    }
}

void MainZone::draw(){
    const auto& adjList = graph.getAdjacencyList();
    for(auto &pair : adjList){
        string u = pair.first;
        if(!nodePos.count(u)) continue;
        for(auto &neighbor : pair.second){
            string v = neighbor.first;
            if(nodePos.count(v)){
                // Draw line only once (since undirected, u-v and v-u exist)
                // Simple check: draw if u < v lexicographically to avoid double drawing
                // if(u < v) DrawLine(nodePos[u].x, nodePos[u].y, nodePos[v].x, nodePos[v].y, GRAY);
            }
        }
    }

    if(currentPath.size()>1){
        for(size_t i=0;i<currentPath.size()-1;i++){
            if(nodePos.count(currentPath[i]) && nodePos.count(currentPath[i+1]))
                DrawLineEx(nodePos[currentPath[i]],nodePos[currentPath[i+1]],8,LIME);
        }
    }

    for(auto &[name,pos]:nodePos){
        Color c=RED;
        if(name==targetNode && !vehicleMoving) c=BLUE;
        else if(name==startNode && !vehicleMoving && !searchBar.searchTriggered) c=GREEN;
        DrawCircle(pos.x,pos.y,20,c);
        DrawCircle(pos.x,pos.y,15,WHITE);
        
        // Aesthetic Text: Boldish (Shadow) + Bigger
        int fontSize = 18;
        int textX = pos.x - MeasureText(name.c_str(), fontSize)/2;
        int textY = pos.y - 40; // Moved up slightly
        
        // Shadow/Outline for readability
        DrawText(name.c_str(), textX+1, textY+1, fontSize, WHITE);
        DrawText(name.c_str(), textX-1, textY-1, fontSize, WHITE);
        DrawText(name.c_str(), textX+1, textY-1, fontSize, WHITE);
        DrawText(name.c_str(), textX-1, textY+1, fontSize, WHITE);
        
        // Main Text
        DrawText(name.c_str(), textX, textY, fontSize, BLACK);
    }

    drawSearchBar();
    drawNavigationInfo();
    if(showLoading) drawLoadingScreen();
}
