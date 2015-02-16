//
//  buildComponentTree.h
//  ProjetComponent
//
//  Created by Emile Mathieu on 14/02/15.
//  Copyright (c) 2015 Emile Mathieu. All rights reserved.
//
#include <vector>
#include <map>
#include <string>
#include <math.h>
using namespace std;

#ifndef ProjetComponent_buildComponentTree_h
#define ProjetComponent_buildComponentTree_h

typedef int Pixel;

struct SetAttribute
{
    Pixel parent;	//	the parent
    int rank;		//	a measure of the depth of the tree
};


class TreeNode{
public:
    double level;
    int area;
    double highest;
    Pixel canonicalElement;
    vector<TreeNode> children;
    TreeNode():level(0),area(1),highest(0),canonicalElement(-1){}
    TreeNode(double levelToAssign, Pixel pixel):level(levelToAssign),area(1),highest(levelToAssign),canonicalElement(pixel){}
    void addChild(const TreeNode & node){ children.push_back(node); }
    void addChilds(const vector<TreeNode> & nodes);
    void display(string prefix= "", string indent= "  ");
};

class CollectionSet{
public:
    map<Pixel, SetAttribute> collectedSet;
    void makeSet(Pixel x);
    Pixel find(Pixel x);
    Pixel link(Pixel x, Pixel y);
};

class Image{
public:
    int rows;
    int cols;
    int elemNum;
    vector<double> pixels;
    vector<int> decreaseOrder;
    Image(vector<double> pixelsValue, int rowsValue, int colsValue);
    void display();
    void displayCrossSections(int lastLevel = 256);
};


#endif
