/*********************************************************
** Filename : buildComponentTree
** Description : The data structure used in component tree algorithm
** Author : Wang Kang
** Email : goto.champion@gmail.com
** Date : 2013/10/5
*********************************************************/
#pragma once
#include <vector>
#include <map>
using namespace std;

typedef int Pixel;

struct SetAttribute
{
	Pixel parent;	//	the parent
	int rank;		//	a measure of the depth of the tree
};

struct TreeNode
{
	double level;
	int area;
	double highest;
	vector<TreeNode> children;

	static TreeNode create(double level)
	{
		TreeNode node;
		node.level = level;
		node.area = 1;
		node.highest = level;
		return node;
	}

	void addChild(const TreeNode & node)
	{
		children.push_back(node);
	}

	void addChilds(const vector<TreeNode> & nodes)
	{
		children.insert(children.end(), nodes.begin(), nodes.end());
	}
};

struct CollectionSet
{
	map<Pixel, SetAttribute> collectedSet;

	//	add the set{x} to the collection Q
	void makeSet(Pixel x)	
	{
		SetAttribute attribute;
		attribute.parent = x;
		attribute.rank = 0;
		collectedSet.insert(pair<Pixel, SetAttribute>(x, attribute));
	}

	//	return the canonical element of the set in Q which contains x
	Pixel find(Pixel x)
	{
		if (collectedSet[x].parent != x)
		{
			collectedSet[x].parent = find(collectedSet[x].parent);
		}
		return collectedSet[x].parent;
	}

	//	let X and Y be the two sets in Q whose canonical elements are x and y (x != y)
	//	both sets are removed from Q, their union Z = X|Y is added to Q and a canonical
	//	element for Z is selected and returned
	Pixel link(Pixel x, Pixel y)
	{
		if (collectedSet[x].rank > collectedSet[y].rank)
		{
			swap(x, y);
		}
		else if (collectedSet[x].rank == collectedSet[y].rank)
		{
			collectedSet[y].rank++;
		}

		collectedSet[x].parent = y;
		return y;
	}

private:
	Pixel & Par(Pixel x)
	{
		return collectedSet[x].parent;
	}

	int & Rnk(Pixel x)
	{
		return collectedSet[x].rank;
	}
};
