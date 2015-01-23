//
//  main.cpp
//  TP6
//
//  Created by Emile Mathieu on 12/01/15.
//  Copyright (c) 2015 Emile Mathieu. All rights reserved.
//
#include <iostream>
#include <vector>
#include "tree.h"
#include "Pixel.h"

void BuildComponentTree(vector<Pixel> image,int numberOfPoints, vector<Tree<Pixel> >& nodes){
    int Fth1 [numberOfPoints];
    int Fth2 [numberOfPoints];
    int subtreeRoot [numberOfPoints];
    for (int i=0; i<numberOfPoints; i++) {
        Fth1[i]=i;
        Fth2[i]=i;
        subtreeRoot[i]=i;
        nodes.push_back(Tree<Pixel>(image[i]));
    }
}

int main() {
    
    vector<Pixel> example;
    example.push_back(Pixel(0,110));
    example.push_back(Pixel(1,90));
    example.push_back(Pixel(2,110));
    example.push_back(Pixel(3,50));
    example.push_back(Pixel(4,50));
    example.push_back(Pixel(5,50));
    example.push_back(Pixel(6,40));
    example.push_back(Pixel(7,20));
    example.push_back(Pixel(8,50));
    example.push_back(Pixel(9,50));
    example.push_back(Pixel(10,50));
    example.push_back(Pixel(11,50));
    example.push_back(Pixel(12,120));
    example.push_back(Pixel(13,70));
    example.push_back(Pixel(14,80));
    
    //std::sort(eleves.begin(),eleves.end(),cmpGrade);
    
    vector<Tree<Pixel> > nodes;
    
    BuildComponentTree(example, 15, nodes);
    
    return 0;
}

