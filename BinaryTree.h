// Binary tree abstract base class
// Created by: Sungjun An
/*  Team project group number 3, Unit 2: BST algorithm by Sungjun An.
*   BinaryTree.h constructs a binary tree object, with functioned implemented to perform inorder traversal-
*   to print, write and destroy the tree.
*/
 
#ifndef _BINARY_TREE
#define _BINARY_TREE

#include "BinaryNode.h"
#include <string>
#include <iostream>

template<class ItemType>
class BinaryTree
{
protected:
    BinaryNode<ItemType>* rootPtr;        // ptr to root node
    int count;                                  // number of nodes in tree

public:
    // "admin" functions
    BinaryTree() {rootPtr = 0; count = 0;}
    BinaryTree(const BinaryTree<ItemType> & tree){ }
    virtual ~BinaryTree() { destroyTree(rootPtr); }
   
    // common functions for all binary trees
    bool isEmpty() const    {return count == 0;}
    int getCount() const {return count;}
    void clear() {destroyTree(rootPtr); rootPtr = 0; count = 0;}
    void inOrder() const  {_inorder(rootPtr);}
    void printTree(void visit(const ItemType &, int)) const{_printTree(visit, rootPtr, 1);}

    // abstract functions to be implemented by derived class
    virtual bool insert(const ItemType &newData) = 0;
    virtual bool remove(const ItemType &target) = 0;
    virtual bool search(const ItemType &target, ItemType & returnedItem) const = 0;

private:
    // delete all nodes from the tree
    void destroyTree(BinaryNode<ItemType>* nodePtr);

    // internal traverse
    void _inorder(BinaryNode<ItemType>* nodePtr) const;
    void _printTree(void visit(const ItemType &, int), BinaryNode<ItemType>* nodePtr, int level) const;
};

//Destroy the entire tree
template<class ItemType>
void BinaryTree<ItemType>::destroyTree(BinaryNode<ItemType>* nodePtr)
{
    if(nodePtr) // != NULL
    {
        destroyTree(nodePtr->getLeftPtr());
        destroyTree(nodePtr->getRightPtr());
        delete nodePtr;
    }
}



//Inorder Traversal
template<class ItemType>
void BinaryTree<ItemType>::_inorder(BinaryNode<ItemType>* nodePtr) const
{
    if (nodePtr) // != NULL
    {
        ItemType item = nodePtr->getItem();
        _inorder(nodePtr->getLeftPtr());
        std::cout << item << std::endl;
        _inorder(nodePtr->getRightPtr());
    }
}



//Prints tree as an indented list
template<class ItemType>
void BinaryTree<ItemType>::_printTree(void visit(const ItemType &, int), BinaryNode<ItemType>* nodePtr, int level) const
{
    if (nodePtr) {
        ItemType item = nodePtr->getItem();
        visit(item, level);
        _printTree(visit, nodePtr->getRightPtr(), level + 1);
        _printTree(visit, nodePtr->getLeftPtr(), level + 1);

    }
}

#endif

