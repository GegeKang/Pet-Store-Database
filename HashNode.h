// This is hashNode.h for hash table.
// Team project group number 3, Unit 3 - Hash, written by: Anita Huang
//The purpose of this file is to provide the setters and getters needed for the variables used in hashtable.h.

#ifndef _HASH_NODE
#define _HASH_NODE

template <class T>
class HashNode
{
private:
    T item;
    int occupied;    // 1 -> occupied, 0 -> empty from start, -1 -> empty after removal
    int noCollisions;

public:
    // constructors
    HashNode() {occupied = 0; noCollisions = 0;}
    HashNode(T anItem) {item = anItem; occupied = 1; noCollisions = 0;}
    HashNode(T anItem, int ocp, int nCol)
             {item = anItem; occupied = ocp; noCollisions = nCol;}
    // setters
    void setItem(const T& anItem) {item = anItem;}
    void setOccupied(int ocp) {occupied = ocp;}
    void setNoCollisions(int nCol) {noCollisions = nCol;}
    
    // getters
    T getItem() const {return item;}
    int getOccupied() const {return occupied;}
    int getNoCollisions() const {return noCollisions;}
};

#endif
