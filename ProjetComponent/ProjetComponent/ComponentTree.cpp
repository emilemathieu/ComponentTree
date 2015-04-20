//
//  buildComponentTree.cpp
//  ProjetComponent
//
//  Created by Emile Mathieu on 14/01/15.
//  Copyright (c) 2015 Emile Mathieu. All rights reserved.
//

#include <iostream>
#include "ComponentTree.h"
#include <map>

// Renvoie les indices des pixels triés par ordre décroissant de leur intensités
vector<int> indexByDecreasingOrder(vector<double> I, int elemNum){
    vector<int> decreaseOrder;
    decreaseOrder.resize(elemNum);
    vector<vector<Pixel>> hist;
    
    int intensitySize = 0;
    for (int i=0; i<elemNum; i++){
        if (I[i]>intensitySize) { intensitySize = I[i]; }
    }
    hist.resize(intensitySize+1);
    
    for (int i=0; i<elemNum; i++){
        hist.at(I[i]).push_back(i);
     }
    int j = 0;
    for (int i=intensitySize; i>=0; i--){
        for (int k=0; k<hist.at(i).size(); k++) {
            decreaseOrder[j]=hist.at(i)[k];
            j++;
        }
    }
    return decreaseOrder;
}

// Ajoute les fils d'un noeuds à un autre noeud
void TreeNode::addChilds(const vector<TreeNode> & nodes){
    children.insert(children.end(), nodes.begin(), nodes.end());
}

// Affiche de manière graphique l'arbre en parcourant de manière récursive les fils
void TreeNode::display(string prefix, string indent){
    cout << prefix << indent << level << endl;
    if (children.size() != 0) {
        indent += "  ";
        for (int i=0; i<children.size(); i++) {
            children[i].display(prefix,indent);
        }
    }
}

// initialise un ensemble par un singleton
void CollectionSet::makeSet(Pixel x){
    SetAttribute attribute;
    attribute.parent = x;
    attribute.rank = 0;
    collectedSet.insert(pair<Pixel, SetAttribute>(x, attribute)); }

// Renvoie l'élement canonique de l'ensemble et utilise le 'path compression' pour gagner en complexité
Pixel CollectionSet::find(Pixel x) {
    if (collectedSet[x].parent != x)
    {
        collectedSet[x].parent = find(collectedSet[x].parent);
    }
    return collectedSet[x].parent;
}

// Fusionne deux ensembles et liant deux pixels
Pixel CollectionSet::link(Pixel x, Pixel y)
{
    if (x != y) {
        
    if (collectedSet[x].rank > collectedSet[y].rank)
    {
        std::swap(x, y);
    }
    else if (collectedSet[x].rank == collectedSet[y].rank)
    {
        collectedSet[y].rank++;
    }
    
    collectedSet[x].parent = y;
    }
    return y;
}

Image::Image(vector<double> pixelsValue, int rowsValue, int colsValue):rows(rowsValue),cols(colsValue),pixels(pixelsValue){
    elemNum = rows * cols;
    decreaseOrder = indexByDecreasingOrder(pixels, elemNum);
}

// Affiche une image en console
void Image::display(){
    for (int j=0; j<rows; j++) {
        for (int i=0; i<cols; i++) {
            if (floor(pixels[j*cols+i]/100)>0) { cout << pixels[j*cols+i] << " "; }
            else if (floor(pixels[j*cols+i]/10)>0) { cout << pixels[j*cols+i] << "  "; }
            else { cout << pixels[j*cols+i] << "    "; }
        }
        cout << endl;
    }
    cout << endl;
}

// Affiche les 'cross sections' d'une image, c'est à dire pour chaque niveau, une image dont tous les pixels ayant une intensité supérieure ou égale à ce niveau sont assignés à 1
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
