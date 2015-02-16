//
//  buildComponentTree.cpp
//  ProjetComponent
//
//  Created by Emile Mathieu on 14/02/15.
//  Copyright (c) 2015 Emile Mathieu. All rights reserved.
//

#include <iostream>
#include "buildComponentTree.h"

vector<int> indexByDecreasingOrder(vector<double> I, int elemNum){
    vector<int> decreaseOrder;
    decreaseOrder.resize(elemNum);
    int* flag = new int [elemNum];
    for (int k=0; k<elemNum; k++) { flag[k]=0; }
    
    for (int i=0; i<elemNum; i++){
        int curMaxValue = 0;
        int curMaxIndex = -1;
        for (int j=0; j<elemNum; j++) {
            
            if (I[j]>curMaxValue && flag[j]==0){
                curMaxValue=I[j];
                curMaxIndex = j;
            }
        }
        flag[curMaxIndex]=1;
        decreaseOrder[i]= curMaxIndex;
    }
    
    return decreaseOrder;
}

void TreeNode::addChilds(const vector<TreeNode> & nodes){
    children.insert(children.end(), nodes.begin(), nodes.end());
}

void TreeNode::display(string prefix, string indent){
    cout << prefix << indent << level << endl;
    if (children.size() != 0) {
        indent += "  ";
        for (int i=0; i<children.size(); i++) {
            children[i].display(prefix,indent);
        }
    }
}

void CollectionSet::makeSet(Pixel x){
    SetAttribute attribute;
    attribute.parent = x;
    attribute.rank = 0;
    collectedSet.insert(pair<Pixel, SetAttribute>(x, attribute)); }

Pixel CollectionSet::find(Pixel x) {
    if (collectedSet[x].parent != x)
    {
        collectedSet[x].parent = find(collectedSet[x].parent);
    }
    return collectedSet[x].parent;
}

Pixel CollectionSet::link(Pixel x, Pixel y)
{
    if (collectedSet[x].rank > collectedSet[y].rank)
    {
        std::swap(x, y);
    }
    else if (collectedSet[x].rank == collectedSet[y].rank)
    {
        collectedSet[y].rank++;
    }
    
    collectedSet[x].parent = y;
    return y;
}

Image::Image(vector<double> pixelsValue, int rowsValue, int colsValue):rows(rowsValue),cols(colsValue),pixels(pixelsValue){
    elemNum = rows * cols;
    decreaseOrder = indexByDecreasingOrder(pixels, elemNum);
}

void Image::display(){
    for (int j=0; j<rows; j++) {
        for (int i=0; i<cols; i++) {
            if (floor(pixels[j*cols+i]/100)>0) { cout << pixels[j*cols+i] << " "; } // test if A(i,f) is made of 1 or 2 digit(s) so as to print the matrix properly
            else if (floor(pixels[j*cols+i]/10)>0) { cout << pixels[j*cols+i] << "  "; }
            else { cout << pixels[j*cols+i] << "    "; }
        }
        cout << endl;
    }
    cout << endl;
}

void Image::displayCrossSections(int lastLevel){
    int newLevel = 0;
    int minLevel = lastLevel;
    
    for (int k=0; k<elemNum; k++) {
        if (pixels[k]>newLevel && pixels[k]<lastLevel) { newLevel = pixels[k]; }
        if (pixels[k]<=minLevel) { minLevel = pixels[k]; }
    }
    cout << "Level: "<<newLevel << endl;
    
    for (int j=0; j<rows; j++) {
        for (int i=0; i<cols; i++) {
            if (pixels[j*cols+i]>=newLevel) { cout << "1" <<"  "; }
            else { cout << "0" <<"  "; }
        }
        cout << endl;
    }
    cout << endl;
    
    if (newLevel!=minLevel) {
        this->displayCrossSections(newLevel);
    }
}
