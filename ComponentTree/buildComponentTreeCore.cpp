/*********************************************************
** Filename : buildComponentTreeCore
** Description : The algorithm to build the component tree
** Reference : L.Najman & M.Couprie - Building the component tree in quasi-linear time
** Author : Wang Kang
** Email : goto.champion@gmail.com
** Date : 2013/10/5
*********************************************************/
#include <stdio.h>
#include <math.h>
#include "mex.h"
#include "buildComponentTree.hpp"

inline void GetProcessedNeighbors(const double* F, int rows, int cols, Pixel p, Pixel* neighbors, int* neighborNum, const int* mask)
{
	Pixel index = p+1;	//	index from 1
	*neighborNum = 0;
    int elemNum = rows * cols;

	//	Top :
	if (index % rows != 1 && F[p-1] >= F[p] && mask[p-1] != 0)
	{
		neighbors[(*neighborNum)++] = p-1;
	}

	//	Bottom :
	if (index % rows != 0 && F[p+1] >= F[p] && mask[p+1] != 0)
	{
		neighbors[(*neighborNum)++] = p+1;
	}

	//	Left :
	if (p - rows >= 0 && F[p - rows] >= F[p] && mask[p - rows] != 0)
	{
		neighbors[(*neighborNum)++] = p - rows;
	}

	//	Right :
	if (p + rows < elemNum && F[p + rows] >= F[p] && mask[p + rows] != 0)
	{
		neighbors[(*neighborNum)++] = p + rows;
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

void PreOrderTree(const TreeNode & root, vector<double> & nodes, vector<pair<Pixel,Pixel> > & adjs)
{
    nodes.push_back(root.level);
    
	const vector<TreeNode> & children = root.children;
    for (vector<TreeNode>::const_iterator iter = children.begin(); iter != children.end(); iter++)
    {
        adjs.push_back(pair<Pixel,Pixel>(root.level, (*iter).level));
        PreOrderTree(*iter, nodes, adjs);
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    //  Get input data
    double* F = mxGetPr(prhs[0]);
    int rows = (int)mxGetM(prhs[0]);
    int cols = (int)mxGetN(prhs[0]);
    int elemNum = rows * cols;
    int* decreaseOrder = (int*)mxGetPr(prhs[1]);
    
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
    for (int p = 0; p < elemNum; p++)
    {
        treeSet.makeSet(p);
        nodeSet.makeSet(p);
        treeNodes[p] = TreeNode::create(F[p]);
        lowestNode[p] = p;
        processedMask[p] = 1;
    }
    
    //  Process
    for (int i = 0; i < elemNum; i++)
    {
        p = decreaseOrder[i];
		curTree = treeSet.find(p);
		curNode = nodeSet.find(lowestNode[curTree]);
        GetProcessedNeighbors(F, rows, cols, p, neighbors, &neighborNum, &(processedMask[0]));
        for (int j = 0; j < neighborNum; j++)
        {
            q = neighbors[j];
            adjTree = treeSet.find(q);
            adjNode = nodeSet.find(lowestNode[adjTree]);
            if (curNode != adjNode)
            {
                if (treeNodes[curNode].level == treeNodes[adjNode].level)
                {
                    curNode = MergeNodes(adjNode, curNode, nodeSet, treeNodes);
                }
				else
				{
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
    
    //  Generate tree export a vector of parent pointers
    vector<double> nodes;
    vector<pair<Pixel,Pixel> > adjs;
    PreOrderTree(treeNodes[treeRoot], nodes, adjs);
    
    //  Output the result
    plhs[0] = mxCreateDoubleMatrix(nodes.size(), 1, mxREAL);
    double* nodeVal = mxGetPr(plhs[0]);
    for (int i = 0; i < nodes.size(); i++)
    {
        nodeVal[i] = nodes[i];
    }
    
    plhs[1] = mxCreateDoubleMatrix(adjs.size(), 2, mxREAL);
    double* adjMat = mxGetPr(plhs[1]);
    for (int i = 0; i < adjs.size(); i++)
    {
        adjMat[i] = adjs[i].first;
        adjMat[i+adjs.size()] = adjs[i].second;
    }
}
