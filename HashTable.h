//HashTable ADT
// Team project group number 3, Unit 3 - Hash, written by: Anita Huang
//This file provides the main functions of the hash algorithm including the
//insert, delete search, and collisions. It uses linear probing to aid with the
//collision resolution solution and finds the longest collision path.

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "HashNode.h"
#include <iostream>

template<class ItemType>
class HashTable
{
private:
   HashNode<ItemType>* hashAry;
   int hashSize;
   int count;
   void _writeHashTableToFile(const std::string&);
    
public:
    HashTable() { count = 0; hashSize = 53; hashAry = new HashNode<ItemType>[hashSize]; }
    HashTable(int n)    { count = 0; hashSize = n;    hashAry = new HashNode<ItemType>[hashSize]; }
    ~HashTable(){ delete [] hashAry;}

   int getCount() const    { return count; }
   int getSize() const { return hashSize; }
   double getLoadFactor() const { return 100.0 * count / hashSize;}
   bool isEmpty() const    { return count == 0; }
   bool isFull()  const    { return count == hashSize; }
    
   bool insert(const ItemType &itemIn, int h(const ItemType &key, int size) );
   bool remove(ItemType &itemOut, const ItemType &key, int h(const ItemType &key, int size));
   int  search(ItemType &itemOut, const ItemType &key, int h(const ItemType &key, int size)) const;
    
   int totalCollisions();
   void longestCollisionPath();
   void writeHashTableToFile(const std::string& fileName){_writeHashTableToFile(fileName);}
   void reHash (int h(const ItemType &key, int size));
};

/*
- Insert an item into the hash table. It avoids duplicates.
- We use linear probing to solve collisions.
- If duplicated data is passed in, return false.
*/
template<class ItemType>
bool HashTable<ItemType>::insert( const ItemType &itemIn, int h(const ItemType &key, int size) )
{
   if ( count == hashSize)
        return false;
    
    int bucket = h(itemIn, hashSize);
    int noCollisions = 0;
    
    while (bucket < hashSize)
    {
        if (hashAry[bucket].getItem() == itemIn && hashAry[bucket].getOccupied() == 1)
            return false;
        
        if (hashAry[bucket].getOccupied() != 1)
        {
            hashAry[bucket].setItem(itemIn);
            hashAry[bucket].setOccupied(1);
            hashAry[bucket].setNoCollisions(noCollisions);
            count++;
            if((100.0 * (count) / hashSize) >= 75.0)
                reHash(h);
            break;
        }
               
        bucket = (bucket+1) % hashSize; // probing
        ++noCollisions;
    }
    return true;
}

/*~*~*~*
   Removes the item with the matching key from the hash table
   if found:
     - copies data in the hash node to itemOut
     - replaces data in the hash node with an empty record (occupied = -1: deleted!)
     - returns true
   if not found:
     - returns false
*~**/
template<class ItemType>
bool HashTable<ItemType>::remove( ItemType &itemOut, const ItemType &key, int h(const ItemType &key, int size))
{
    int bucket = h(key, hashSize);
    int noCollisions = search(itemOut, key,h);
    
    if (noCollisions != -1 && bucket < hashSize) // item existed
    {
        while (hashAry[bucket].getOccupied() != 0) // occupied or empty after removal
        {
            if (hashAry[bucket].getItem() == key && hashAry[bucket].getOccupied() == 1)
            {
                hashAry[bucket].setOccupied(-1);
                count--;
                return true;
            }
            bucket = (bucket+1) % hashSize;
        }
    }
    
    return false;
}

/*~*~*~*
   hash search - linear probe
   if found:
      - copy data to itemOut
      - returns the number of collisions for this key
   if not found, returns -1
*~**/
template<class ItemType>
int HashTable<ItemType>::search(ItemType &itemOut, const ItemType &key, int h(const ItemType &key, int size)) const
{
    int noCollisions = 0;
    int bucket = h(key,hashSize);
    
    while (bucket < hashSize && hashAry[bucket].getOccupied() != 0) // occupied or empty after removal
    {
        if (hashAry[bucket].getItem() == key && hashAry[bucket].getOccupied() == 1)
        {
            itemOut = hashAry[bucket].getItem();
            return noCollisions;
        }
        bucket = (bucket+1) % hashSize;
        ++noCollisions;
    }
    
    return -1;
}

/*
 This function calculates the total collision amount.
 To achieve that, the function traverses each index and counts collisions of each node in occupied bucket.
 */
template<class ItemType>
int HashTable<ItemType>::totalCollisions()
{
    int collisionCount = 0;
    int i = 0;
    while (i < hashSize)
    {
        if (hashAry[i].getOccupied() == 1)
        {
            collisionCount += hashAry[i].getNoCollisions();
        }
        ++i;
    }
    return collisionCount;
}

/*
 This function finds the longest collisions path.
 - To achieve that, the function traverses each index and finds the maximum collisions and its maxIndex.
 - Once got the maximum node and its current maxIndex, use hash function again to get its original index - i.
 - From index i to maxIndex, all can be counted as collisions.
 - Finally, print this path.
 */
template<class ItemType>
void HashTable<ItemType>::longestCollisionPath()
{
    int maxIndex = 0, i = 1;
    while (i < hashSize)
    {
        if (hashAry[i].getNoCollisions() > hashAry[maxIndex].getNoCollisions())
        {
            maxIndex = i;
        }
        ++i;
    }
    
    ItemType longestCollisionNode = hashAry[maxIndex].getItem();
    
    std::cout << "The longest collision path is: ";
    
    i = key_to_index(longestCollisionNode, hashSize);
    for (; i <=maxIndex; i++)
    {
        if (hashAry[i].getOccupied() == 1)
            std::cout << hashAry[i].getItem().getOwner() << "  ";
    }
    std::cout << std::endl;
}

/*
- This function is to Write to file in hashtable sequence.
- By using a for loop, we can get the hashtable sequence.
- To write a pet object into a file directly, we overloaded the ofstream insertion operator in Pet.h
- Team project group number 3, Unit 5 - File I/O, written by: Gezheng Kang & Juhwan Park
 */
template<class ItemType>
void HashTable<ItemType>::_writeHashTableToFile(const std::string& fileName)
{
    std::ofstream outFile(fileName);
    if (!outFile.is_open())
    {
         std::cerr << "Error opening file: " << fileName << std::endl;
         return;
     }

     for (int i = 0; i < hashSize; i++)
    {
        if (hashAry[i].getOccupied() == 1)
        {
            outFile << hashAry[i].getItem();
        }
    }

     outFile.close();
 }


/*
- This function is to allocate more memory for the hashAry when the current load factor is great than or euqal to 75.0
- If resize/rehash is needed, create a new bigger hashAry,
  traverse and rehash each occupied bucket in the previous array, and then put them into the new array.
  Finally, update the hashAry, new count,and noCollisions. Delete the previous hashAry.
- Team project group number 3, Unit 5 - File I/O, written by: Gezheng Kang & Juhwan Park
 */
template<class ItemType>
void HashTable<ItemType>::reHash(int h(const ItemType &key, int size))
{
    int newSize = 2 * hashSize;
    HashNode<ItemType>* newAry = new HashNode<ItemType>[newSize];

    for (int i = 0; i < hashSize; i++)
    {
        if (hashAry[i].getOccupied() == 1)
        {
            ItemType item = hashAry[i].getItem();
            int bucket = h(item, newSize);
            int noCollisions = 0;

            while (bucket < newSize)
            {
                if (newAry[bucket].getOccupied() != 1)
                {
                    newAry[bucket].setItem(item);
                    newAry[bucket].setOccupied(1);
                    newAry[bucket].setNoCollisions(noCollisions);
                    break;
                }

                bucket = (bucket + 1) % newSize; // probing
                ++noCollisions;
            }
        }
    }

    delete[] hashAry;
    hashAry = newAry;
    hashSize = newSize; // update the array and size
}





#endif

