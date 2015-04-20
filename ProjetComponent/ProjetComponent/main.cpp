#include <stdio.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <queue>
#include <map>
#include "ComponentTree.h"

// Renvoi les voisins d'un pixel
inline void GetProcessedNeighbors(const vector<double> F, int rows, int cols, Pixel p, Pixel* neighbors, int* neighborNum, vector<int> mask)
{
	Pixel index = p+1;
	*neighborNum = 0;
    int elemNum = rows * cols;

    //	Au dessus :
    // Vérifie qu'on ne sort pas de l'image, qu'il a déja été parcouru et que son intensité est supérieur ou égale à celle du pixel que l'on traite
    if (p - cols >= 0 && F[p - cols] >= F[p] && mask[p - cols] != 0)
    {
        neighbors[(*neighborNum)++] = p - cols;
    }
    //	En dessous :
    if (p + cols < elemNum && F[p + cols] >= F[p] && mask[p + cols] != 0)
    {
        neighbors[(*neighborNum)++] = p + cols;
    }
    //	Gauche :
    if (index % cols != 1 && F[p-1] >= F[p] && mask[p-1] != 0)
    {
        neighbors[(*neighborNum)++] = p - 1;
    }
    //	Droite :
    if (index % cols != 0 && F[p+1] >= F[p] && mask[p+1] != 0)
    {
        neighbors[(*neighborNum)++] = p + 1;
    }
}

// Fusionne deux noeuds
Pixel MergeNodes(Pixel x, Pixel y, CollectionSet & nodeSet, vector<TreeNode> & treeNodes)
{
	Pixel t1 = nodeSet.link(x, y), t2;
	if (t1 == y)
	{
		treeNodes[y].addChilds(treeNodes[x].children);
		t2 = x;
	}
	else
	{
		treeNodes[x].addChilds(treeNodes[y].children);
		t2 = y;
	}
	treeNodes[t1].area += treeNodes[t2].area; // Met à jour les attributs: aire
	treeNodes[t1].highest = max(treeNodes[t1].highest, treeNodes[t2].highest); // et intensité maximale
	return t1;
}

// Reconstruit l'image à partir de l'abre et des composantes
Image reconstructImager(int rows, int cols,vector<TreeNode> treeNodes, CollectionSet nodeSet, TreeNode treeRoot, int minimumArea=0){
    int elemNum = rows * cols;
    vector<double> F;
    F.resize(elemNum);
    map<Pixel,int> mapping;
    for (int k=0; k<elemNum; k++) { // initialise les valeurs
        F[k]=0;
    }
    
    queue<TreeNode> pile; // On travaille avec une queue à laquelle on ajoute sucessivement les fils des composantes
    pile.push(treeRoot);
    
    while (!pile.empty()) { // Tant que la queue n'est pas vide il reste des composantes à traiter
        TreeNode curNode = pile.front(); // composante que l'on regrde actuellement
        pile.pop();
        
        for (int k=0; k< curNode.children.size(); k++) {
            if (curNode.children[k].area>=minimumArea) { // Filtre sur l'aire de la composante
                pile.push(curNode.children[k]); // Si l'aire convient on ajoute le fils à la queue
            }
            else {
                pile.push(TreeNode(curNode.level, curNode.children[k].canonicalElement)); // Sinon on rajoute on modifie le niveau du fils puis on le rajoute
            }
        }
        mapping.insert(pair<Pixel, int>(curNode.canonicalElement, curNode.level));
    }
    
    for (int i=0; i<elemNum; i++){
        F[i]=mapping[nodeSet.find(i)];
    }
    return Image(F,rows,cols);
}

void ComponentAlgorithm(Image image, bool verbose=false, int minimumArea=0) {
    
    //  Récupère les données nécessaires
    vector<double> F = image.pixels;
    int rows = image.rows;
    int cols = image.cols;
    vector<int> decreaseOrder = image.decreaseOrder;
    int elemNum = rows * cols;
    
    //  Déclarations
    CollectionSet treeSet; // permet de savoir a quel arbre partiel chaque point appartient, utile uniquement pendant l'algorithme
    CollectionSet nodeSet; // permet de savoir à quelle composante chaque pixel appartient
	vector<TreeNode> treeNodes; //contient l'arbre des composantes
	vector<Pixel> subtreeRoot; // permet de retrouver la racine 'réelle' des arbres partielles, s'utilise avec l'élement renvoyé par treeSet
	vector<int> processed; // garde en mémoire le fait d'avoir déja traité un pixel
	Pixel p, q, curCanonicalElt, curNode, adjCanonicalElt, adjNode, neighbors[4];
	int neighborNum = 0;
 
    treeNodes.resize(elemNum);
    subtreeRoot.resize(elemNum);
    processed.resize(elemNum);
 
    //  Initialisation
    for (int p = 0; p < elemNum; p++) {
        treeSet.makeSet(p); // initialise chaque ensemble comme un singleton
        nodeSet.makeSet(p); // idem
        treeNodes[p] = TreeNode(F[p],p);
        subtreeRoot[p] = p;
        processed[p] = 0;
    }
 
    //  Coeur de l'algorithme
    for (int i = 0; i < elemNum; i++) { // boucle sur les pixels de l'image dans l'ordre d'intensité décroissante
        p = decreaseOrder[i]; // Récupère un pixel
        if (verbose) {cout << "Step " << i << ": Pixel " << p << endl;}
        curCanonicalElt = treeSet.find(p); // Récupère le pixel qui représente l'arbre partiel
        curNode = nodeSet.find(subtreeRoot[curCanonicalElt]); // Récupère l'élément canonique du noeud de la racine 'réelle' de l'arbre partielle (grâce à subtreeRoot)
        GetProcessedNeighbors(F, rows, cols, p, neighbors, &neighborNum, processed); // Récupère les voisins de p
        for (int j = 0; j < neighborNum; j++) { // boucle sur les voisins
            q = neighbors[j];
            if (verbose) {cout << "  -Neighboor Pixel " << q <<": ";}
            adjCanonicalElt = treeSet.find(q); // Fait de même que pour p
            adjNode = nodeSet.find(subtreeRoot[adjCanonicalElt]); // idem
            if (curNode != adjNode) {
                if (treeNodes[curNode].level == treeNodes[adjNode].level) { // si les noeuds sont de même niveau, on les fusionne
                    if (verbose) {cout << "Nodes "<< curNode <<" & "<< adjNode<<" merged";}
                    curNode = MergeNodes(curNode, adjNode, nodeSet, treeNodes);
                    if (verbose) {cout << " (" << curNode << " as CE)";}
                }
                else { // Sinon on rattache les fils de adjNode à ceux de curNode
                    if (verbose) {cout << adjNode << " and his children added to " <<curNode;}
                    treeNodes[curNode].addChild(treeNodes[adjNode]);
                    treeNodes[curNode].area += treeNodes[adjNode].area;
                    treeNodes[curNode].highest = max(treeNodes[curNode].highest, treeNodes[adjNode].highest);
                }
            }
            else {
                if (verbose) {cout << "Same nodes";}
            }
            curCanonicalElt = treeSet.link(adjCanonicalElt, curCanonicalElt); //Lie les deux pixels
            subtreeRoot[curCanonicalElt] = curNode;
            if (verbose) {cout << endl;}
        }
        processed[p] = 1; // enregistre le fait que le pixel ait déjà été traité
    }
 
 
	Pixel treeRoot = subtreeRoot[treeSet.find(nodeSet.find(0))]; //  Récupère la racine de l'arbre
    treeNodes[treeRoot].display("* "); // affiche l'arbre

    reconstructImager(rows,cols,treeNodes,nodeSet,treeNodes[treeRoot],1).display();

 }

int main(){
    
    const double arrF[]={110, 90, 100,50, 50, 50,40, 20, 50,50, 50, 50,120, 70, 80};
    vector<double> F(arrF,arrF+sizeof(arrF)/sizeof(arrF[0]));
    
    Image image1 = Image(F,5,3);
    
    //image1.display();
    //image1.displayCrossSections();
    ComponentAlgorithm(image1,true);
        
    const double arrG[]={1,1,1,1,1,1,1,1,3,3,2,3,4,1,1,3,3,2,3,4,1,1,1,1,1,1,3,1,1,3,3,2,1,1,1,1,4,3,2,2,2,1,1,1,1,1,1,1,1};
    vector<double> G(arrG,arrG+sizeof(arrG)/sizeof(arrG[0]));
    Image image2 = Image(G,7,7);
    //image2.display();
    //image2.displayCrossSections();
    ComponentAlgorithm(image2,false,2);
        
    return 0;
}
