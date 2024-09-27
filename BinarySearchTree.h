// Binary Search Tree ADT
// Created by: Sungjun AN
/*
    Team project group number 3, Unit 2: BST algorithm by Sungjun An.
    In this project, the BST algorithm successfully constructs a Binary tree of nodes of data containing
    each pet object's attributes with a key being the owner's name in ASCII value.
    The order which the tree gets inserted and organized is determined by that value.
    This file contains basic algorithm operation for; Inorder traversal of Search, Insertion and Removal of a leaf or a node.

*/
 
#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE

#include "BinaryTree.h"

template<class ItemType>
class BinarySearchTree : public BinaryTree<ItemType>
{
public:
    // insert a node at the correct location
    bool insert(const ItemType &item);
    bool search(const ItemType &target, ItemType &returnedItem) const;
    bool remove(const ItemType& target);
    
private:
    // internal insert node: insert newNode in nodePtr subtree
    BinaryNode<ItemType>* _insert(BinaryNode<ItemType>* nodePtr, BinaryNode<ItemType>* newNode);
   
    // search for target node
    BinaryNode<ItemType>* _search(BinaryNode<ItemType>* treePtr, const ItemType &target) const;

   // internal remove node: locate and delete target node under nodePtr subtree
    BinaryNode<ItemType>* _remove(BinaryNode<ItemType>* nodePtr, const ItemType& target, bool& isSuccessful);
    
};


///////////////////////// public function definitions ///////////////////////////
// Wrapper for _insert - Inserting items within a tree
template<class ItemType>
bool BinarySearchTree<ItemType>::insert(const ItemType & newEntry)
{
    BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(newEntry);
    
    this->rootPtr = _insert(this->rootPtr, newNodePtr);
    this->count += 1;
    return true;
}

// Wrapper for _search
// - it calls the private _search function that returns a Node pointer or NULL
// - if found, it copies data from that node and sends it back to the caller
//   via the output parameter, and returns true, otherwise it returns false.
template<class ItemType>
bool BinarySearchTree<ItemType>::search(const ItemType& anEntry, ItemType & returnedItem) const
{
    BinaryNode<ItemType>* temp = nullptr;
   
    temp = _search(this->rootPtr, anEntry);
    if (temp != nullptr) {
        returnedItem = temp->getItem();
        return true;
    }
    return false;
}
// Wrapper for _remove
// Calls the private _remove function
// decreases count(number of elements in bst) if success.
// this function returns true when removal was a success, false otherwise.
template<class ItemType>
bool BinarySearchTree<ItemType>::remove(const ItemType& target)
{
    bool isSuccessful = false;
    this->rootPtr = _remove(this->rootPtr, target, isSuccessful);

    if (isSuccessful) {
        this->count--;
    }

    return isSuccessful;
    
}

//////////////////////////// private functions ////////////////////////////////////////////

// Iterative implementation of the insert operation
// returns pointer to the node that got inserted as a new node.
// if the tree is empty, make the newNode as the root.
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_insert(BinaryNode<ItemType>* nodePtr,
                                                          BinaryNode<ItemType>* newNodePtr)
{
    BinaryNode<ItemType>* pWalk = nodePtr, *parent = nullptr;
    
    if( !nodePtr) // == NULL
    {
        nodePtr = newNodePtr;
        return nodePtr;
    }
    else
    {
        while(pWalk) // != NULL
        {
            parent = pWalk;
            if (pWalk->getItem() > newNodePtr->getItem())
                pWalk = pWalk->getLeftPtr();
            else
                pWalk = pWalk->getRightPtr();
        }
        if (parent->getItem() > newNodePtr->getItem())
            parent->setLeftPtr(newNodePtr);
        else
            parent->setRightPtr(newNodePtr);
    }

    return nodePtr;
}

// Recursive implementation of the search operation
// - return NULL if target not found, otherwise
// - returns a pointer to the node that matched the target
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_search(BinaryNode<ItemType>* nodePtr,const ItemType &target) const
{
    BinaryNode<ItemType>* found = nullptr;

    if (nodePtr == nullptr)
    {
        return found;
    }
    if (nodePtr->getItem() > target)
    {
        return _search(nodePtr->getLeftPtr(), target);
    }
    else if (nodePtr->getItem() < target)
    {
        return _search(nodePtr->getRightPtr(), target);
    }
    else
    {
        found = nodePtr;
    }
    return found;
    
}

// Recursive implementation of the remove operation
// - return null if target not found, otherwise
// - returns a pointer to the node that matched the target
// - if a deleted node has a child, it will promote the child accordingly.

template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::_remove(BinaryNode<ItemType>* nodePtr, const ItemType& target, bool& isSuccessful)
{
    if (nodePtr == nullptr) // if the tree is empry, root is NULL.
    {
        isSuccessful = false;
        return nullptr;
    }

    // found the targeted node to be deleted
    if (nodePtr->getItem() > target)
    {
        nodePtr->setLeftPtr(_remove(nodePtr->getLeftPtr(), target, isSuccessful));
    }
    
    else if (nodePtr->getItem() < target)
    {
        nodePtr->setRightPtr(_remove(nodePtr->getRightPtr(), target, isSuccessful));
    }
    
    else
    {
        // Node to be deleted has been found, delPtr is the target-node.
        BinaryNode<ItemType>* delPtr = nodePtr;

        // Node has no children, simply delete and update variables
        if (nodePtr->getLeftPtr() == nullptr && nodePtr->getRightPtr() == nullptr)
        {
            delete delPtr;
            isSuccessful = true;
            return nullptr;
        }
        
        // Node has only right child, simply delete the original one and make the right child as parent.
        // Return the new-parent node(right child before delete) to the old-ancester/grandparent of the right child to update the child of grandparent. Here are 3 "dimension"/"level" update.
        else if (nodePtr->getLeftPtr() == nullptr)
        {
            nodePtr = nodePtr->getRightPtr(); // update the parent node of the subtree - right child becomes the parent
            delPtr->setRightPtr(nullptr);
            delete delPtr;
            isSuccessful = true;
            return nodePtr;
        }
        
        // Node has only left child, simply delete the original one and make the left child as parent.
        // Return the new-parent node(left child before delete) to the old-ancester/grandparent of the left child to update the child of grandparent. Here are 3 "dimension"/"level" update.
        else if (nodePtr->getRightPtr() == nullptr)
        {
            nodePtr = nodePtr->getLeftPtr();
            delPtr->setLeftPtr(nullptr);
            delete delPtr;
            isSuccessful = true;
            return nodePtr;
        }
        
        // Node has both left and right children,a successor node is selected from the right subtree. The successor is the leftmost node in the right subtree (the node with the smallest value greater than the current node).
        // Then, just assign the value of the leftmost to current node/ delPtr
        // Then, recursively call _remove to delete the duplicate node with value of leftmostData.
        //The updated right child pointer is returned to update the parent's child pointer.
        else
        {
            //leftmost node in the right subtree line# 213-217
            BinaryNode<ItemType>* successor = nodePtr->getRightPtr();
            while (successor->getLeftPtr() != nullptr)
            {
                successor = successor->getLeftPtr();
            }
            
            nodePtr->setItem(successor->getItem());
            nodePtr->setRightPtr(_remove(nodePtr->getRightPtr(), successor->getItem(), isSuccessful));
        }
    }

    return nodePtr;
}


#endif
