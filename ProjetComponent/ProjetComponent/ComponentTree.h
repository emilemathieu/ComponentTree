//
//  buildComponentTree.h
//  ProjetComponent
//
//  Created by Emile Mathieu on 14/01/15.
//  Copyright (c) 2015 Emile Mathieu. All rights reserved.
//
#include <vector>
#include <map>
#include <string>
#include <math.h>
using namespace std;

// Définition du type Pixel qui sera utilisé dans l'algorithme
typedef int Pixel;

struct SetAttribute
{
    int rank; // est une mesure de la profondeur de l'arbre, est nécessaire pour 'Union by rank' qui améliore la complexité
    Pixel parent;	// parent du pixel
    
};

// Représente une composante connexe
class TreeNode{
public:
    double level; // Niveau/Intensité de la composante
    int area; //Aire
    double highest; //Intensité maximale des composantes connectées
    Pixel canonicalElement; // Pixel représentant la composante
    vector<TreeNode> children; // Composantes du niveau juste supérieur qui sont incluse dans cette composante
    TreeNode():level(0),area(1),highest(0),canonicalElement(-1){}
    TreeNode(double levelToAssign, Pixel pixel):level(levelToAssign),area(1),highest(levelToAssign),canonicalElement(pixel){}
    void addChild(const TreeNode & node){ children.push_back(node); }
    void addChilds(const vector<TreeNode> & nodes);
    void display(string prefix= "", string indent= "  "); // Affiche l'arbre
};

// Modélise la relation d'équivalence des pixels de l'image (Union-Find)
class CollectionSet{
public:
    map<Pixel, SetAttribute> collectedSet;
    void makeSet(Pixel x);
    Pixel find(Pixel x); // Renvoie l'élement canonique représentant l'ensemble
    Pixel link(Pixel x, Pixel y); // Fusionne deux ensembles
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