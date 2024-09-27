// Binary Node abstract base class
// Created by: Sungjun An
/* Team project group number 3, Unit 2: BST algorithm by Sungjun An.
*  BinaryNode.h declared a node object within the tree with attributes; pointers to right and left child nodes.
*/
#ifndef _BINARY_NODE
#define _BINARY_NODE

template<class ItemType>
class BinaryNode
{
private:
    ItemType              item;        // Data portion
    BinaryNode<ItemType>* leftPtr;     // Pointer to left child
    BinaryNode<ItemType>* rightPtr;    // Pointer to right child

public:
    // constructors
    BinaryNode(const ItemType & anItem)    {item = anItem; leftPtr = 0; rightPtr = 0;}
    BinaryNode(const ItemType & anItem,
               BinaryNode<ItemType>* left,
               BinaryNode<ItemType>* right) {item = anItem; leftPtr = left; rightPtr = right;}
    // setters
    void setItem(const ItemType & anItem) {item = anItem;}
    void setLeftPtr(BinaryNode<ItemType>* left) {leftPtr = left;}
    void setRightPtr(BinaryNode<ItemType>* right) {rightPtr = right;}
    
    // getters
    ItemType getItem() const     {return item;}
    BinaryNode<ItemType>* getLeftPtr() const  {return leftPtr;}
    BinaryNode<ItemType>* getRightPtr() const {return rightPtr;}
    

    
};

#endif
