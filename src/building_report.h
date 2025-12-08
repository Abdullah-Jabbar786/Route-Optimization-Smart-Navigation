#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <iostream>
#include <map>
using namespace std;

class TimeUnit {
    public:
    string name; 
    bool isCategory;
    vector<TimeUnit*> children;

    TimeUnit(string n, bool isCat) : name(n), isCategory(isCat) {}
    ~TimeUnit() {
        for (auto child : children) 
        {
            delete child;
        }
    }
};


class ReportNode {
    public:
    string name; 
    bool isCategory;
    vector<ReportNode*> children;

    ReportNode(string n, bool isCat) : name(n), isCategory(isCat) {}
    ~ReportNode() {
        for (auto child : children) 
        {
            delete child;
        }
    }
};

class BuildingReport {
private:
    ReportNode* root;

    ReportNode* findCategory(string category);
    void printNode(ReportNode* node, int level);
    void searchRecursive(ReportNode* node, string buildingName, vector<string>& path, bool& found);
   
public:
    BuildingReport();
    ~BuildingReport();

    void insert(string category, string buildingName);
    bool search(string buildingName);
    int countBuildings(string category);
    void printGroupedReport();
    void draw(int x, int y); 
    string findCategoryOfBuilding(string buildingName); 
};
