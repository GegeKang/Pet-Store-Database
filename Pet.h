/* Specification file for the Pet class
- overloaded the relational operators and ofstream insertion operator
- definie a friend hash function to get bucket
- definie three friend display functions
- Team project group number 3, Unit 1, written by Team Leader: Gezheng Kang
*/
#ifndef Pet_h
#define Pet_h
#include <iostream>
#include <fstream>
using std::ostream;
using std::string;

class Pet; // Forward Declaration

// Function Prototypes for friend functions
int key_to_index(const Pet &, int);
void hDisplay(const Pet &);
void vDisplay(const Pet &);
std::ofstream& operator << (std::ofstream& strm, const Pet&);

class Pet
{
private:
    string owner;  // the key for hash
    string name;   // the name of the pet
    string type;   // the type of the pet
    double age;    // the age of the pet
    double weight; // the weight of the pet
    
public:
    // constructor
    Pet()
    {
        owner = "";
        name = "";
        type = "";
        age = 0.0;
        weight = 0.0;
    }
    Pet(string owner, string name, string type, double age, double weight)
    {
        this->owner = owner;
        this->name = name;
        this->type = type;
        this->age = age;
        this->weight = weight;
    }
    // setters
    void setOwner(string o) { owner = o; }
    void setName(string n) { name = n; }
    void setType(string t) { type = t; }
    void setWeight(double w) { weight = w; }
    void setAge(double a) { age = a; }
    
    // getters
    string getOwner() const { return owner; }
    string getName() const { return name; }
    string getType() const { return type; }
    double getWeight() const { return weight; }
    double getAge() const { return age; }
    
    // Friend Functions Definitions
    friend int key_to_index(const Pet &key, int size)
    {
        string k = key.getOwner();
        int sum = 0;
        for (int i = 0; k[i]; i++)
            sum += k[i];
        return sum % size;
    }

    // hDisplay
    // Displays code, state, and year data members of a Park object
    // on one line (horizontal display)
    friend void hDisplay(const Pet &item)
    {
        std::cout << item.owner  << " has one ";
        std::cout << item.type  << " named ";
        std::cout << item.name << ".";

        std::cout << std::endl;
    }

    // vDisplay
    // Displays name, description, state, and year of a Park object
    // one per line (vertical display)
    friend void vDisplay (const Pet &item)
    {
        std::cout << "       Owner: " << item.owner << std::endl;
        std::cout << "        Name: " << item.name << std::endl;
        std::cout << "        Type: " << item.type << std::endl;
        std::cout << "         Age: " << item.age << " yrs" << std::endl;
        std::cout << "      Weight: " << item.weight << " lb" << std::endl;
    }
    
    // Cause we are required to insert data into the hashed array and BST based on a unique key, overload operators here.
    // overloading <, compare two objects by their owners, as owners are unqie keys
    bool operator<(const Pet &p)
    {
        if (this->owner < p.owner)
            return true;
        else
            return false;
    }
    // overloading >, compare two objects by their owners
    bool operator>(const Pet &p)
    {
        if (*this < p || (*this == p))
            return false;
        else
            return true;
    }
    // overloading ==, check whether they are equal to each other
    bool operator==(const Pet &p)
    {
        if (this->owner == p.owner)
            return true;
        else
            return false;
    }
    
    // This overloaded operator is used for writing data to a file.
    // The format matches that of the read-from file, so that we can read again.
    friend std::ofstream& operator << (std::ofstream& strm, const Pet& pet)
    {
        strm << pet.owner << ": " << pet.name << ", " << pet.age << " " << pet.type << " " << pet.weight << std::endl;
        return strm;
    }
    
};
#endif /* Pet_h */
