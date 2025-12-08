#include "building_report.h"
#include <iostream>
#include <algorithm>

using namespace std;

BuildingReport::BuildingReport() 
{
    root = new ReportNode("Root", true);
}

BuildingReport::~BuildingReport() 
{
    delete root;
}

ReportNode* BuildingReport::findCategory(string category) 
{
    for (auto child : root->children) 
    {
        if (child->isCategory && child->name == category) 
        {
            return child;
        }
    }
    return nullptr;
}

void BuildingReport::insert(string category, string buildingName)
{
    ReportNode* catNode = findCategory(category);
    if (!catNode) 
    {
        catNode = new ReportNode(category, true);
        root->children.push_back(catNode);
    }
    
    for(auto child : catNode->children) 
    {
        if(child->name == buildingName) return;
    }

    ReportNode* buildingNode = new ReportNode(buildingName, false);
    catNode->children.push_back(buildingNode);
}

void BuildingReport::searchRecursive(ReportNode* node, string buildingName, vector<string>& path, bool& found) 
{
    if (found) return;

    if (!node->isCategory && node->name == buildingName) 
    {
        found = true;
        cout << "Found \"" << buildingName << "\" in category: " << path.back() << endl;
        return;
    }

    if (node->isCategory && node != root) 
    {
        path.push_back(node->name);
    }

    for (auto child : node->children) 
    {
        searchRecursive(child, buildingName, path, found);
        if (found) return;
    }

    if (node->isCategory && node != root) 
    {
        path.pop_back();
    }
}

bool BuildingReport::search(string buildingName) 
{
    vector<string> path;
    bool found = false;
    searchRecursive(root, buildingName, path, found);
    if (!found) 
    {
        cout << "Building \"" << buildingName << "\" not found." << endl;
    }
    return found;
}

int BuildingReport::countBuildings(string category) 
{
    ReportNode* catNode = findCategory(category);
    if (!catNode) return 0;
    return catNode->children.size();
}

string BuildingReport::findCategoryOfBuilding(string buildingName) 
{
    if (buildingName.empty()) return "";
    
    for (auto catNode : root->children) 
    {
        for (auto bNode : catNode->children) 
        {
            if (bNode->name == buildingName) return catNode->name;
        }
    }
    return "";
}


void BuildingReport::printGroupedReport(){
    cout << "\n--- Building Group Report ---\n";
    for (auto catNode : root->children) {
        cout << "\n" << catNode->name << ":\n";
        for (auto buildingNode : catNode->children) {
            cout << " - " << buildingNode->name << "\n";
        }
    }
    cout << "-----------------------------\n";
}

void BuildingReport::draw(int startX, int startY) 
{
    int currentY = startY;
    int currentX = startX;
    int indent = 30;
    int categoryFontSize = 24;
    int itemFontSize = 20;

    const char* title = "Building Configuration Report";

    DrawText(title, startX+2, startY - 38, 30, LIGHTGRAY);

    DrawText(title, startX, startY - 40, 30, DARKBLUE); 

    int maxY = 800; 

    for (auto catNode : root->children) 
    {

        int neededHeight = 28 + (catNode->children.size() * 22) + 10;
        
        if (currentY + neededHeight > maxY) 
        {
            currentY = startY;
            currentX += 280; 
        }

        DrawText(catNode->name.c_str(), currentX+1, currentY+1, categoryFontSize, LIGHTGRAY);
        DrawText(catNode->name.c_str(), currentX, currentY, categoryFontSize, MAROON); 
        
        string countText = "(" + to_string(catNode->children.size()) + ")";
        DrawText(countText.c_str(), currentX + MeasureText(catNode->name.c_str(), categoryFontSize) + 5, currentY + 4, 16, BLACK);
        
        currentY += 28;

        for (auto buildingNode : catNode->children) 
        {
            DrawCircle(currentX + indent - 10, currentY + 10, 3, DARKBLUE);
            DrawText(buildingNode->name.c_str(), currentX + indent, currentY, itemFontSize, DARKGRAY);
            currentY += 22;
        }
        currentY += 10; 
    }
}