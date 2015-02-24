#include <stdio.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <queue>
#include "buildComponentTree.h"

inline void GetProcessedNeighbors(const vector<double> F, int rows, int cols, Pixel p, Pixel* neighbors, int* neighborNum, vector<int> mask)
{
	Pixel index = p+1;	//	index from 1
	*neighborNum = 0;
    int elemNum = rows * cols;

    //	Top :
    if (p - cols >= 0 && F[p - cols] >= F[p] && mask[p - cols] != 0)
    {
        neighbors[(*neighborNum)++] = p - cols;
    }
    
    //	Bottom :
    if (p + cols < elemNum && F[p + cols] >= F[p] && mask[p + cols] != 0)
    {
        neighbors[(*neighborNum)++] = p + cols;
    }
    
    //	Left :
    if (index % cols != 1 && F[p-1] >= F[p] && mask[p-1] != 0)
    {
        neighbors[(*neighborNum)++] = p - 1;
    }
    
    //	Right :
    if (index % cols != 0 && F[p+1] >= F[p] && mask[p+1] != 0)
    {
        neighbors[(*neighborNum)++] = p + 1;
    }
}

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
	treeNodes[t1].area += treeNodes[t2].area;
	treeNodes[t1].highest = max(treeNodes[t1].highest, treeNodes[t2].highest);
	return t1;
}

/*void PreOrderTree(const TreeNode & root, vector<double> & nodes, vector<pair<Pixel,Pixel> > & adjs)
{
    nodes.push_back(root.level);
    
	const vector<TreeNode> & children = root.children;
    for (vector<TreeNode>::const_iterator iter = children.begin(); iter != children.end(); iter++)
    {
        adjs.push_back(pair<Pixel,Pixel>(root.level, (*iter).level));
        PreOrderTree(*iter, nodes, adjs);
    }
}*/

Image reconstructImage(int rows, int cols,vector<TreeNode> treeNodes, CollectionSet nodeSet, TreeNode treeRoot, int minimumArea=0){
    int elemNum = rows * cols;
    vector<double> F;
    F.resize(elemNum);
    vector<int> flag;
    flag.resize(elemNum);
    for (int k=0; k<elemNum; k++) {
        flag[k]=0;
        F[k]=0;
    }
    
    queue<TreeNode> pile;
    pile.push(treeRoot);
    
    while (!pile.empty()) {
        TreeNode curNode = pile.front();
        pile.pop();
        
        for (int i=0; i<elemNum; i++){
            if (flag[i]==0 && nodeSet.find(i)==curNode.canonicalElement) {
                F[i]=curNode.level;
                flag[i]=1;
            }
        }
        for (int k=0; k< curNode.children.size(); k++) {
            if (curNode.children[k].area>=minimumArea) {
                pile.push(curNode.children[k]);
            }
            else {
                pile.push(TreeNode(curNode.level, curNode.children[k].canonicalElement));
            }
        }
    }
    return Image(F,rows,cols);
}

void ComponentAlgorithm(Image image) {
    
    //  Get input data
    vector<double> F = image.pixels;
    int rows = image.rows;
    int cols = image.cols;
    vector<int> decreaseOrder = image.decreaseOrder;
    int elemNum = rows * cols;
    
    //  Useful data
    CollectionSet treeSet;
    CollectionSet nodeSet;
	vector<TreeNode> treeNodes;
	vector<Pixel> lowestNode;
	vector<int> processedMask;
	Pixel p, q, curTree, curNode, adjTree, adjNode, neighbors[4];
	int neighborNum = 0;
 
    treeNodes.resize(elemNum);
    lowestNode.resize(elemNum);
    processedMask.resize(elemNum);
 
    //  Pre-process
    for (int p = 0; p < elemNum; p++) {
        treeSet.makeSet(p);
        nodeSet.makeSet(p);
        treeNodes[p] = TreeNode(F[p],p);
        lowestNode[p] = p;
        processedMask[p] = 0;
    }
 
    //  Process
    for (int i = 0; i < elemNum; i++) {
        cout << "Step " << i << endl;
        p = decreaseOrder[i];
        if (p==3) {
            
        }
        curTree = treeSet.find(p);
        curNode = nodeSet.find(lowestNode[curTree]);
        GetProcessedNeighbors(F, rows, cols, p, neighbors, &neighborNum, processedMask);
        for (int j = 0; j < neighborNum; j++) {
            q = neighbors[j];
            adjTree = treeSet.find(q);
            adjNode = nodeSet.find(lowestNode[adjTree]);
            if (curNode != adjNode) {
                if (treeNodes[curNode].level == treeNodes[adjNode].level) {
                    curNode = MergeNodes(adjNode, curNode, nodeSet, treeNodes);
                }
                else {
                    treeNodes[curNode].addChild(treeNodes[adjNode]);
                    treeNodes[curNode].area += treeNodes[adjNode].area;
                    treeNodes[curNode].highest = max(treeNodes[curNode].highest, treeNodes[adjNode].highest);
                }
            }
            curTree = treeSet.link(adjTree, curTree);
            lowestNode[curTree] = curNode;
        }
        processedMask[p] = 1;
    }
 
 //  Get the root of tree
	Pixel treeRoot = lowestNode[treeSet.find(nodeSet.find(0))];
    //treeNodes[nodeSet.find(decreaseOrder.back())].display("* ");
    treeNodes[treeRoot].display("* ");

    reconstructImage(rows,cols,treeNodes,nodeSet,treeNodes[treeRoot],3).display();
    
 //  Generate tree export a vector of parent pointers
 //vector<double> nodes;
 //vector<pair<Pixel,Pixel> > adjs;
 //PreOrderTree(treeNodes[treeRoot], nodes, adjs);
     
 }

int main(){
    
    vector<double> F = vector<double> {110, 90, 100,50, 50, 50,40, 20, 50,50, 50, 50,120, 70, 80};
    Image image1 = Image(F,5,3);
    image1.display();
    //image1.displayCrossSections();
    ComponentAlgorithm(image1);
    
    vector<double> G = vector<double> {1,1,1,1,1,1,1,1,3,3,2,3,4,1,1,3,3,2,3,4,1,1,1,1,1,1,3,1,1,3,3,2,1,1,1,1,4,3,2,2,2,1,1,1,1,1,1,1,1};
    Image image2 = Image(G,7,7);
    //image2.display();
    //image2.displayCrossSections();
    //ComponentAlgorithm(image2);
        
    return 0;
}
