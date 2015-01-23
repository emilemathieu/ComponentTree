//
//  tree.h
//  TP6
//
//  Created by Emile Mathieu on 12/01/15.
//
//

#ifndef _TREE_H_
#define _TREE_H_

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
//#include <algorithm>
using namespace std;


template <class T>
class Tree {
    T data;
    vector<Tree<T>*> sons;
public:
    Tree<T>();
    // Create a node with given information
    Tree<T>(T d);
    // Return information of this node
    T getData();
    // Set information of this node
    void setData(T d);
    // Return the number of sons of this node
    int nbSons();
    //Return the son at position pos (left-most at position 0)
    Tree<T>* getSon(int pos);
    // Set the son at position pos (left-most at position 0)
    void setSon(int pos, Tree<T>* tree);
    // Add tree as supplementary right-most son of this node
    void addAsLastSon(Tree<T>* tree);
    // Remove right-most son of this node
    void removeLastSon();
    // Add new son at position pos
    void addSon(int pos, Tree<T>* son);
    // Remove son at position pos
    void removeSon(int pos);
    int maxDepth(int depth = 0);
    int minDepth();
    void display(string prefix = "", string indent = "  ");
    void BFSdisplay(string prefix = "", string indent = "  ");
};

template <class T>
Tree<T>::Tree():data(0){}

template <class T>
Tree<T>::Tree(T d):data(d){}

template <class T>
T Tree<T>::getData(){ return data;}

template <class T>
void Tree<T>::setData(T d){
    data = d;
}

template <class T>
int Tree<T>::nbSons() {
    return sons.size();
}

template <class T>
Tree<T>* Tree<T>::getSon(int pos){
    // If the integer 'pos' is out of bound relatively to the size of the vector 'sons', then the NULL pointeur is returned
    if (pos < 0 || pos >= this->nbSons()) {
        return NULL;
    }
    return sons[pos];
}

template <class T>
void Tree<T>::setSon(int pos, Tree<T>* tree){
    // If the integer 'pos' is out of bound relatively to the size of the vector 'sons', then a string exception is thrown
    if (pos < 0 || pos >= this->nbSons()) {
        throw "indice out of bound";
    }
    sons[pos]= tree;
}

template <class T>
void Tree<T>::addAsLastSon(Tree<T>* tree){
    sons.push_back(tree);
}

template <class T>
void Tree<T>::removeLastSon(){
    // If there no son to be removed, then a string exception is thrown
    if (this->nbSons()==0) {
        throw "no son to remove";
    }
    sons.pop_back();
}

template <class T>
void Tree<T>::addSon(int pos, Tree<T>* son){
    // If the integer 'pos' is out of bound relatively to the size of the vector 'sons', then a string exception is thrown
    if (pos < 0 || pos >= this->nbSons()) {
        throw "indice out of bound";
    }
    sons.insert(sons.begin()+pos, son);
}

template <class T>
void Tree<T>::removeSon(int pos){
    // If the integer 'pos' is out of bound relatively to the size of the vector 'sons', then a string exception is thrown
    if (pos < 0 || pos >= this->nbSons()) {
        throw "indice out of bound";
    }
    sons.erase(sons.begin()+pos);
}

template <class T>
int Tree<T>::maxDepth(int depth){
    if (this->nbSons() == 0) {
        return depth;
    }
    else{
        depth++;
        vector<int> maxValues;
        for (int i=0; i<this->nbSons(); i++) {
            maxValues.push_back(this->sons[i]->maxDepth(depth));
        }
        
        return *(max_element(maxValues.begin(), maxValues.end()));
    }
}

template <class T>
int Tree<T>::minDepth(){
    if (this->nbSons() == 0) {
        return 0;
    }
    queue<Tree<T>*> nodes;
    queue<int> level;
    nodes.push(this);
    level.push(0);
    while (nodes.empty()==false) {
        Tree<T>* mytree = nodes.front();
        nodes.pop();
        int count = level.front();
        level.pop();
        if (mytree->nbSons()==0) {
            return count;
        }
        for (int i=0; i<mytree->nbSons(); i++) {
            nodes.push(mytree->getSon(i));
            level.push(count+1);
        }
    }
    return -1;
}

template <class T>
void Tree<T>::display(string prefix, string indent){
    cout << prefix << indent << data << endl;
    if (this->nbSons() != 0) {
        indent += "  ";
        for (int i=0; i<this->nbSons(); i++) {
            this->getSon(i)->display(prefix,indent);
        }
    }
}

template <class T>
void Tree<T>::BFSdisplay(string prefix, string indent){
    queue<Tree<T>*> queue;
    queue.push(this);
    while (queue.empty()==false) {
        Tree<T>* mytree = queue.front();
        cout << mytree->getData() << " ";
        queue.pop();
        for (int i=0; i<mytree->nbSons(); i++) {
            queue.push(mytree->getSon(i));
        }
    }
    cout << endl;
    
}

#endif


